#ifndef BASIC_SOCKET_H
#define BASIC_SOCKET_H

#include "endpoint.h"
#include "socket-option.h"

class basic_socket_t
{
public:
    virtual int native() const = 0;
    virtual void bind(const endpoint_t &endpoint) = 0;
    virtual size_t recv(void *buffer, size_t len) = 0;
    virtual size_t sendto(const endpoint_t &endpoint, const void *buffer, size_t len) = 0;
    virtual void set_option(const socket_option_t &opt) = 0;
    virtual ~basic_socket_t() = default;
};

#endif // BASIC_SOCKET_H
