#include "udp-socket.h"
#include "posix-error-check.h"

#include <unistd.h>

udp_socket_t::udp_socket_t()
    : fd_(check_error(::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP), "socket"))
{}

udp_socket_t::~udp_socket_t()
{
    ::close(fd_);
}

int udp_socket_t::native() const
{
    return fd_;
}

void udp_socket_t::bind(const endpoint_t &endpoint)
{
    sockaddr_in addr = get_sockaddr(endpoint);
    int rc = ::bind(fd_, (const sockaddr*)&addr, sizeof(addr));
    check_error(rc, "bind");
}

size_t udp_socket_t::recv(void *buffer, size_t len)
{
    ssize_t bytes = ::recv(fd_, buffer, len, 0);
    check_error(bytes, "recv");
    return static_cast<size_t>(bytes);
}

size_t udp_socket_t::sendto(const endpoint_t &endpoint, const void *buffer, size_t len)
{
    sockaddr_in addr = get_sockaddr(endpoint);
    ssize_t written = ::sendto(fd_, buffer, len, 0, (const sockaddr*)&addr, sizeof(addr));
    check_error(written, "sendto");
    return static_cast<size_t>(written);
}

void udp_socket_t::set_option(const socket_option_t &opt)
{
    const auto &value = opt.value();
    int rc = ::setsockopt(fd_, opt.level(), opt.name(), value.data(), value.size());
    check_error(rc, "setsockopt");
}

sockaddr_in udp_socket_t::get_sockaddr(const endpoint_t &endpoint)
{
    const auto &ipv4_endpoint = endpoint_cast(endpoint);

    sockaddr_in addr{};
    addr.sin_family = ipv4_endpoint.family();
    addr.sin_addr.s_addr = htonl(ipv4_endpoint.address());
    addr.sin_port = htons(ipv4_endpoint.port());

    return addr;
}

const ipv4_endpoint_t& udp_socket_t::endpoint_cast(const endpoint_t &endpoint)
{
    if (endpoint.family() != AF_INET) {
        throw std::system_error(EAFNOSUPPORT, std::generic_category());
    }

    return static_cast<const ipv4_endpoint_t&>(endpoint);
}
