#include <getopt.h>
#include "Network.h"

int main(int argc, char **argv) {
    std::string host = "localhost";
    int port = 13050;
    std::string reservation;

    while (true) {
        const auto opt = getopt_long(argc, argv, "h:p:r:", (const option[]) {
                {"host",        required_argument, nullptr, 'h'},
                {"port",        required_argument, nullptr, 'p'},
                {"reservation", required_argument, nullptr, 'r'},
                {nullptr,       no_argument,       nullptr, 0}
        }, nullptr);

        if (-1 == opt) {
            break;
        }

        switch (opt) {
            case 'h':
                host = std::string(optarg);
                break;
            case 'p':
                port = std::stoi(optarg);
                break;
            case 'r':
                reservation = std::string(optarg);
                break;
            default:
                break;
        }
    }

    // Connect to server
    Network network{host, port};

    // Join game
    if (reservation.empty()) {
        network.joinAnyGame();
    } else {
        network.joinReservation(reservation);
    }

    // Play game
    network.gameLoop();

    // Disconnect from server
    network.close();

    return 0;
}
