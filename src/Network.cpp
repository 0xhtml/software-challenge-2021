#include "Network.h"

#include <bitset>
#include <boost/asio.hpp>
#include <chrono>
#include <iostream>
#include <string>
#include <pugixml.hpp>

#include "Strings.h"
#include "Types.h"

Network::Network(const std::string &host, const int port) {
    boost::asio::ip::tcp::endpoint endpoint;

    if (host == "localhost") {
        endpoint.address(boost::asio::ip::make_address("127.0.0.1"));
    } else {
        boost::asio::ip::tcp::resolver::query query{host, ""};
        endpoint = *boost::asio::ip::tcp::resolver(ioService).resolve(query);
    }

    endpoint.port(port);

    socket.connect(endpoint);
    send("<protocol>");
}

void Network::send(const std::string &data) {
    boost::system::error_code error;
    write(socket, boost::asio::buffer(data), error);
}

std::string Network::receive() {
    boost::system::error_code error;

    std::size_t bytes = read_until(socket, receiveBuffer, "</room>", error);

    if (!error) {
        std::string data{
                boost::asio::buffers_begin(receiveBuffer.data()),
                boost::asio::buffers_begin(receiveBuffer.data()) + bytes
        };
        receiveBuffer.consume(bytes);
        return data;
    }

    return "";
}

void Network::joinAnyGame() {
    send("<join gameType=\"swc_2021_blokus\"/>");
}

void Network::joinReservation(const std::string &reservation) {
    send("<joinPrepared reservationCode=\"" + reservation + "\" />");
}

void normalizeMoveVariation(Move &move) {
    // Some moves are precisely the same due to symmetric pieces.
    // Those moves are not supported and therefore have to be normalized.
    switch (move.piece) {
        case 0:
        case 6:
        case 16:
            move.rotation = 0;
            move.flipped = 0;
            break;
        case 1:
        case 3:
        case 4:
        case 9:
            move.rotation %= 2;
            move.flipped = 0;
            break;
        case 2:
        case 12:
        case 14:
            if (move.rotation >= 2) {
                move.rotation = (move.rotation - 1) % 2;
                move.flipped = !move.flipped;
            }
            break;
        case 8:
        case 17:
            move.rotation %= 2;
            break;
        case 7:
        case 10:
        case 19:
            switch (move.rotation) {
                case 0:
                case 2:
                    move.flipped = 0;
                    break;
                case 3:
                    move.rotation = 1;
                    move.flipped = !move.flipped;
                    break;
            }
            break;
    }
}

Move parseMove(const pugi::xml_node &xmlMove) {
    pugi::xml_node xmlMovePiece = xmlMove.child("piece");
    pugi::xml_node xmlMovePosition = xmlMovePiece.child("position");

    Move move{};

    move.color = STR_INDEX(COLOR_NAMES, xmlMovePiece.attribute("color").value());
    move.piece = STR_INDEX(PIECE_NAMES, xmlMovePiece.attribute("kind").value());
    move.rotation = STR_INDEX(ROTATION_NAMES, xmlMovePiece.attribute("rotation").value());
    move.flipped = STR_INDEX(FLIPPED_NAMES, xmlMovePiece.attribute("isFlipped").value());
    normalizeMoveVariation(move);

    // NOTE: X and Y is intentionally flipped
    move.x = xmlMovePosition.attribute("y").as_int();
    move.y = xmlMovePosition.attribute("x").as_int();

    return move;
}

void Network::parseGameState(const pugi::xml_node &xmlGameState) {
    gameState.firstPiece = STR_INDEX(PIECE_NAMES, xmlGameState.attribute("startPiece").value());

    pugi::xml_node xmlMove = xmlGameState.child("lastMove");
    if (xmlMove && xmlMove.attribute("class").value() == std::string{"sc.plugin2021.SetMove"}) {
        gameState.performMove(parseMove(xmlMove));
    }

    // Set turn manually to account for skip moves
    gameState.turn = xmlGameState.attribute("turn").as_int();
}

std::string Network::moveToXML(const Move &move) {
    std::string xml = "<room roomId=\"";

    xml.append(roomId);
    xml.append("\"><data class=\"sc.plugin2021.SetMove\"><piece color=\"");
    xml.append(COLOR_NAMES[move.color]);
    xml.append("\" kind=\"");
    xml.append(PIECE_NAMES[move.piece]);
    xml.append("\" rotation=\"");
    xml.append(ROTATION_NAMES[move.rotation]);
    xml.append("\" isFlipped=\"");
    xml.append(FLIPPED_NAMES[move.flipped]);

    // NOTE: X and Y is intentionally flipped
    xml.append("\"><position x=\"");
    xml.append(std::to_string(move.y));
    xml.append("\" y=\"");
    xml.append(std::to_string(move.x));

    xml.append("\"/></piece></data></room>");

    return xml;
}

void Network::gameLoop() {
    bool running = true;

    while (running) {
        std::string data = receive();
        Time start = std::chrono::system_clock::now();

        if (data.empty()) {
            printf("data is empty\n");
            break;
        }

        if (data.find("<protocol>") == std::string::npos) {
            data.insert(0, "<protocol>");
        }

        if (data.find("</protocol>") == std::string::npos) {
            data.append("</protocol>");
        } else {
            printf("data includes '</protocol>'\n");
            break;
        }

        pugi::xml_document xmlDocument;
        xmlDocument.load_string(data.data());

        pugi::xml_node xml = xmlDocument.child("protocol");

        pugi::xml_node joined = xml.child("joined");
        if (joined) {
            roomId = joined.attribute("roomId").value();
        }

        for (pugi::xml_node roomMessage : xml.children("room")) {
            pugi::xml_node roomMessageData = roomMessage.child("data");
            std::string roomMessageDataClass = roomMessageData.attribute("class").value();

            if (roomMessageDataClass == "sc.framework.plugins.protocol.MoveRequest") {
#ifndef TESTING
                printf("       --- TURN %2d ---\n", gameState.turn);
#endif

                Move move = algorithm.iterativeDeepening(gameState, start);

                if (move.color >= COLOR_COUNT) {
                    move = gameState.getPossibleMoves()[0];
                }

                send(moveToXML(move));

#ifndef TESTING
                Time now = std::chrono::system_clock::now();
                int time = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
                printf("     --- TIME %4dms ---\n\n", time);
#endif
            } else if (roomMessageDataClass == "memento") {
                parseGameState(roomMessageData.child("state"));
            } else if (roomMessageDataClass == "result") {
                pugi::xml_node winner = roomMessageData.child("winner");

                printf("WINNER %s", winner.attribute("displayName").value());
                printf("%s\n", winner.child("color").text().get());

                running = false;
            } else if (roomMessageDataClass == "error") {
                printf("ERROR %s\n", roomMessageData.attribute("message").value());
                running = false;
            } else {
                printf("data includes '%s'\n", roomMessageDataClass.c_str());
            }
        }
    }
}

void Network::close() {
    send("<sc.protocol.responses.CloseConnection />");
    send("</protocol>");

    boost::system::error_code error;
    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
    socket.close(error);

    std::cout << std::endl;
    for (int i = 0; i < COLOR_COUNT; ++i) {
        for (U32 row : gameState.board[i]) {
            printf("%05x ", row);
        }
        std::cout << std::endl;
    }
    for (auto &deployedPieces : gameState.deployedPieces) {
        for (bool piece : deployedPieces) {
            std::cout << std::to_string(piece);
        }
        std::cout << " ";
    }
    std::cout << std::endl;
}
