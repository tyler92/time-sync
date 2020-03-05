#ifndef CANSOCKET_H
#define CANSOCKET_H

#include "basic-socket.h"
#include <linux/can.h>

class can_socket_t : public basic_socket_t
{
public:
    can_socket_t();
    ~can_socket_t();

    int native() const override;
    void bind(const endpoint_t &endpoint) override;
    size_t recv(void *buffer, size_t len) override;
    size_t sendto(const endpoint_t &endpoint, const void *buffer, size_t len) override;
    void set_option(const socket_option_t &opt) override;

private:
    sockaddr_can get_sockaddr(const endpoint_t &endpoint);
    static const can_endpoint_t& endpoint_cast(const endpoint_t &endpoint);

private:
    int fd_;
};

#endif // CANSOCKET_H
