#include "socket-factory.h"

#include "udp-socket.h"

#ifndef NO_CAN_SOCKETS
#include "can-socket.h"
#endif

#include <system_error>

socket_factory_t::socket_factory_t()
{
    register_socket(AF_INET, []{ return new udp_socket_t(); });

#ifndef NO_CAN_SOCKETS
    register_socket(AF_CAN, []{ return new can_socket_t(); });
#endif
}

basic_socket_t* socket_factory_t::create_broadcast(const std::string &url,
                                                   broadcast_mode mode) const
{
    std::unique_ptr<endpoint_t> endpoint(endpoint_t::create(url));
    return create_broadcast(*endpoint, mode);
}

basic_socket_t* socket_factory_t::create_broadcast(const endpoint_t &endpoint,
                                                   broadcast_mode mode) const
{
    const auto found = creators_.find(endpoint.family());
    if (found == creators_.end()) {
        throw std::system_error(EAFNOSUPPORT, std::generic_category());
    }

    std::unique_ptr<basic_socket_t> socket(found->second());

    switch (mode) {
        case broadcast_mode::client:
            socket->set_option(reuse_address_option_t());
            socket->bind(endpoint);
            break;
        case broadcast_mode::server:
            socket->set_option(broadcast_option_t());
            break;
    }

    return socket.release();
}

void socket_factory_t::register_socket(int family, const socket_factory_t::creator_t &creator)
{
    creators_[family] = creator;
}

void socket_factory_t::unregister_all()
{
    creators_.clear();
}
