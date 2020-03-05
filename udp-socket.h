#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "basic-socket.h"
#include <netinet/in.h>

class udp_socket_t : public basic_socket_t
{
public:
    udp_socket_t();
    ~udp_socket_t();

    int native() const override;
    void bind(const endpoint_t &endpoint) override;
    size_t recv(void *buffer, size_t len) override;
    size_t sendto(const endpoint_t &endpoint, const void *buffer, size_t len) override;
    void set_option(const socket_option_t &opt) override;

private:
    static sockaddr_in get_sockaddr(const endpoint_t &endpoint);
    static const ipv4_endpoint_t& endpoint_cast(const endpoint_t &endpoint);

private:
    int fd_;
};

#endif // UDPSOCKET_H
