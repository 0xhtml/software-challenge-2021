#pragma once

#include <boost/asio.hpp>
#include <pugixml.hpp>
#include "Algorithm.h"

using namespace boost::asio;

/**
 * Class to connect to the server and handle the network communication
 */
class Network {
private:
    io_service ioService;
    ip::tcp::socket socket{ioService};
    streambuf receiveBuffer;
    boost::system::error_code error;
    std::string roomId;
    GameState gameState{};
    Algorithm algorithm{};

    /**
     * Send data to server. Wrapper for boost::asio::write()
     * @param data String containing the data to be written
     */
    void send(const std::string &data);

    /**
     * Receive a room message (inclusive up to '</room>') from the server
     * @return String containing the room message or an empty string if receive failed
     */
    std::string receive();

    /**
     * Parse a received game state and update gameState
     * @param xmlGameState <state> xml node out of memento
     */
    void parseGameState(const pugi::xml_node &xmlGameState);

    /**
     * Generate XML response to send a move to the server
     * @param move Move
     * @return String containing the XML response to be send
     */
    std::string moveToXML(const Move &move);

public:
    /**
     * Initialize the connection to the server by connecting and sending '<protocol>'
     * @param host String containing the server hostname
     * @param port Int containing the server port
     */
    Network(const std::string &host, int port);

    /**
     * Join any game available game. Sends <join>-request
     */
    void joinAnyGame();

    /**
     * Join a reserved game. Sends <joinPrepared>-request
     * @param reservation String containing the reservation code
     */
    void joinReservation(const std::string &reservation);

    /**
     * Handle a game including calls to Algorithm and parseGameState()
     */
    void gameLoop();

    /**
     * Close the connection by sending a <CloseConnection>-request, '</protocol>'
     * and shutting down as well as closing the socket
     */
    void close();
};
