#include "broadcast-sender.h"
#include "timestamp-packet.h"

#include <unistd.h>
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cout << "Usage: time-sync-server <endpoint>\n\n"
                     "    example: time-sync-server udp://192.168.0.255:8801\n"
                     "    example: time-sync-server can://vcan1\n";
        return 1;
    }

    try {
        broadcast_sender_t sender(argv[1]);

        for (;;) {
            sender.send(timestamp_packet_t{});
            usleep(100 * 1000);
        }
    } catch (const std::exception &ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
