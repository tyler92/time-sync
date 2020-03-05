#ifndef BROADCASTSENDER_H
#define BROADCASTSENDER_H

#include "net-packet.h"
#include "socket-factory.h"

#include <memory>

class endpoint_t;
class basic_socket_t;

class broadcast_sender_t
{
public:
    broadcast_sender_t(const std::string &url, const socket_factory_t &factory = {});
    ~broadcast_sender_t();

    size_t send(const net_packet_t &packet);
    size_t send(const void *buffer, size_t len);

private:
    std::unique_ptr<endpoint_t> endpoint_;
    std::unique_ptr<basic_socket_t> socket_;
};

#endif // BROADCASTSENDER_H
