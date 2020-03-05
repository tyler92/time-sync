#ifndef BROADCASTRECEIVER_H
#define BROADCASTRECEIVER_H

#include "net-packet.h"
#include "socket-factory.h"

#include <memory>

class broadcast_receiver_t
{
public:
    broadcast_receiver_t(const std::string &url, const socket_factory_t &factory = {});
    ~broadcast_receiver_t();

    size_t receive(void *buffer, size_t len);
    size_t receive(net_packet_t &packet);

private:
    std::unique_ptr<basic_socket_t> socket_;
};

#endif // BROADCASTRECEIVER_H
