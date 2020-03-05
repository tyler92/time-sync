#include "broadcast-receiver.h"
#include "timestamp-packet.h"
#include "time-printer.h"

#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cout << "Usage: time-sync-client <endpoint>\n\n"
                     "    example: time-sync-client udp://0.0.0.0:8801\n"
                     "    example: time-sync-client can://vcan1\n";
        return 1;
    }

    try {
        broadcast_receiver_t receiver(argv[1]);

        for (;;) {
            timestamp_packet_t packet;
            receiver.receive(packet);

            const auto &server_time = packet.payload();
            const auto &client_time = std::chrono::system_clock::now();

            time_printer_t printer;
            printer.print_to(std::cout, server_time, client_time);
        }
    } catch (const std::exception &ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
