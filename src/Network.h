#pragma once

#include <boost/asio.hpp>
#include <string>
#include <pugixml.hpp>

#include "GameState.h"
#include "Algorithm.h"
#include "Types.h"

class Network {
private:
    boost::asio::io_service ioService;
    boost::asio::ip::tcp::socket socket{ioService};
    boost::asio::streambuf receiveBuffer;
    std::string roomId;
    GameState gameState{};
    Algorithm algorithm{};

    void send(const std::string &data);

    std::string receive();

    void parseGameState(const pugi::xml_node &xmlGameState);

    std::string moveToXML(const Move &move);

public:
    Network(const std::string &host, int port);

    void joinAnyGame();

    void joinReservation(const std::string &reservation);

    void gameLoop();

    void close();
};
