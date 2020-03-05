#include "can-socket.h"
#include "posix-error-check.h"

#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>

can_socket_t::can_socket_t()
    : fd_(check_error(::socket(PF_CAN, SOCK_RAW, CAN_RAW), "socket"))
{}

can_socket_t::~can_socket_t()
{
    ::close(fd_);
}

int can_socket_t::native() const
{
    return fd_;
}

void can_socket_t::bind(const endpoint_t &endpoint)
{
    sockaddr_can addr = get_sockaddr(endpoint);
    int rc = ::bind(fd_, (const sockaddr*)&addr, sizeof(addr));
    check_error(rc, "bind");
}

size_t can_socket_t::recv(void *buffer, size_t len)
{
    struct can_frame frame;
    ssize_t bytes = ::recv(fd_, &frame, sizeof(frame), 0);
    check_error(bytes, "recv");

    constexpr size_t can_header_len = sizeof(frame) - CAN_MAX_DLEN;

    // short header
    if (bytes <= can_header_len) {
        return 0;
    }

    // short body
    if (bytes < frame.can_dlc + can_header_len) {
        return 0;
    }

    std::memcpy(buffer, frame.data, frame.can_dlc);
    return static_cast<size_t>(frame.can_dlc);
}

size_t can_socket_t::sendto(const endpoint_t &endpoint, const void *buffer, size_t len)
{
    if (len > CAN_MAX_DLEN) {
        throw std::out_of_range("buffer is too long");
    }

    struct can_frame frame{};
    frame.can_id = 1;
    frame.can_dlc = len;
    std::memcpy(&frame.data, buffer, len);

    sockaddr_can addr = get_sockaddr(endpoint);

    ssize_t written = ::sendto(fd_, &frame, sizeof(frame), 0, (const sockaddr*)&addr, sizeof(addr));
    check_error(written, "sendto");
    return static_cast<size_t>(written);
}

void can_socket_t::set_option(const socket_option_t &opt)
{
    const auto &value = opt.value();
    int rc = ::setsockopt(fd_, opt.level(), opt.name(), value.data(), value.size());
    check_error(rc, "setsockopt");
}

sockaddr_can can_socket_t::get_sockaddr(const endpoint_t &endpoint)
{
    const auto &can_endpoint = endpoint_cast(endpoint);

    struct ifreq ifr;
    std::strcpy(ifr.ifr_name, can_endpoint.device());

    int rc = ioctl(fd_, SIOCGIFINDEX, &ifr);
    check_error(rc, "ioctl");

    sockaddr_can addr{};
    addr.can_family = can_endpoint.family();
    addr.can_ifindex = ifr.ifr_ifindex;

    return addr;
}

const can_endpoint_t& can_socket_t::endpoint_cast(const endpoint_t &endpoint)
{
    if (endpoint.family() != AF_CAN) {
        throw std::system_error(EAFNOSUPPORT, std::generic_category());
    }

    return static_cast<const can_endpoint_t&>(endpoint);
}

