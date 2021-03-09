#include "Network.h"
#include "Strings.h"

Network::Network(const std::string &host, int port) {
    ip::tcp::endpoint endpoint;

    if (host == "localhost") {
        endpoint.address(ip::make_address("127.0.0.1"));
    } else {
        endpoint = *ip::tcp::resolver(ioService).resolve(ip::tcp::resolver::query(host, ""));
    }

    endpoint.port(port);

    socket.connect(endpoint);
    send("<protocol>");
}

void Network::send(const std::string &data) {
    write(socket, buffer(data));
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

Move parseMove(pugi::xml_node xmlMove) {
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

void Network::parseGameState(pugi::xml_node xmlGameState) {
    gameState.firstPiece = STR_INDEX(PIECE_NAMES, xmlGameState.attribute("startPiece").value());

    pugi::xml_node xmlMove = xmlGameState.child("lastMove");
    if (xmlMove && xmlMove.attribute("class").value() == std::string{"sc.plugin2021.SetMove"}) {
        gameState.performMove(parseMove(xmlMove));
    }

    for (bool &gameOver : gameState.gameOver) {
        gameOver = true;
    }
    for (pugi::xml_node xmlColor : xmlGameState.child("validColors").children("color")) {
        gameState.gameOver[STR_INDEX(COLOR_NAMES, xmlColor.text().get())] = false;
    }

    // Set turn manually to account for skip moves
    gameState.turn = xmlGameState.attribute("turn").as_int();
}

std::string Network::moveToXML(Move move) {
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
        if (data.empty()) {
            // Receive failed; end connection
            break;
        }

        if (data.find("<protocol>") == std::string::npos) {
            data.insert(0, "<protocol>");
        }

        if (data.find("</protocol>") == std::string::npos) {
            data.append("</protocol>");
        } else {
            // Data closes protocol; end connection
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

            if (roomMessageDataClass == "memento") {
                parseGameState(roomMessageData.child("state"));
            } else if (roomMessageDataClass == "sc.framework.plugins.protocol.MoveRequest") {
                Move move = algorithm.iterativeDeepening(gameState);

                if (move.color >= COLOR_COUNT) {
                    move = gameState.getPossibleMoves()[0];
                }

                send(moveToXML(move));
            } else if (roomMessageDataClass != "welcomeMessage") {
                // Received a room message like result or error; end connection
                running = false;
                break;
            }
        }
    }
}

void Network::close() {
    send("<sc.protocol.responses.CloseConnection />");
    send("</protocol>");
    socket.shutdown(ip::tcp::socket::shutdown_both);
    socket.close();
}
