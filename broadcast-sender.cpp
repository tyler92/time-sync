#include "broadcast-sender.h"
#include "endpoint.h"

broadcast_sender_t::broadcast_sender_t(const std::string &url,
                                       const socket_factory_t &factory)
    : endpoint_(endpoint_t::create(url))
    , socket_(factory.create_broadcast(*endpoint_, broadcast_mode::server))
{}

broadcast_sender_t::~broadcast_sender_t() = default;

size_t broadcast_sender_t::send(const net_packet_t &packet)
{
    std::vector<char> buf = packet.serialize();
    return send(buf.data(), buf.size());
}

size_t broadcast_sender_t::send(const void *buffer, size_t len)
{
    return socket_->sendto(*endpoint_, buffer, len);
}
