#include "Network.h"
#include "Strings.h"

Network::Network(const std::string &address, int port) {
    socket.connect(ip::tcp::endpoint(ip::make_address(address), port));
    send("<protocol>");
}

void Network::send(std::string data) {
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

Move parseMove(pugi::xml_node xmlMove) {
    pugi::xml_node xmlMovePiece = xmlMove.child("piece");
    pugi::xml_node xmlMovePosition = xmlMovePiece.child("position");

    Move move{};

    move.color = STR_INDEX(COLOR_NAMES, xmlMovePiece.attribute("color").value());
    move.piece = STR_INDEX(PIECE_NAMES, xmlMovePiece.attribute("kind").value());
    move.rotation = STR_INDEX(ROTATION_NAMES, xmlMovePiece.attribute("rotation").value());
    move.flipped = STR_INDEX(FLIPPED_NAMES, xmlMovePiece.attribute("isFlipped").value());

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
                send(moveToXML(algorithm.iterativeDeepening(gameState)));
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
