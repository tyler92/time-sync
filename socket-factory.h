#ifndef SOCKETFACTORY_H
#define SOCKETFACTORY_H

#include "basic-socket.h"

#include <map>
#include <string>
#include <functional>

enum class broadcast_mode
{
    server,
    client
};

class socket_factory_t
{
public:
    socket_factory_t();

    basic_socket_t* create_broadcast(const std::string &url, broadcast_mode mode) const;
    basic_socket_t* create_broadcast(const endpoint_t &endpoint, broadcast_mode mode) const;

    using creator_t = std::function<basic_socket_t*()>;
    void register_socket(int family, const creator_t &creator);

    void unregister_all();

private:
    std::map<int, creator_t> creators_;
};

#endif // SOCKETFACTORY_H
