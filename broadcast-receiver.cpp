#include "broadcast-receiver.h"

broadcast_receiver_t::broadcast_receiver_t(const std::string &url,
                                           const socket_factory_t &factory)
    : socket_(factory.create_broadcast(url, broadcast_mode::client))
{}

broadcast_receiver_t::~broadcast_receiver_t() = default;

size_t broadcast_receiver_t::receive(void *buffer, size_t len)
{
    return socket_->recv(buffer, len);
}

size_t broadcast_receiver_t::receive(net_packet_t &packet)
{
    size_t size = packet.byte_size();
    std::vector<char> buf(size);

    size = socket_->recv(buf.data(), size);
    return packet.deserialize(buf.data(), size);
}
