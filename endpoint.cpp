#include "endpoint.h"

#include <cstring>
#include <limits>

#include <netinet/in.h>

template <class T>
static bool srt_to_number(const std::string &str, T &value, size_t &last)
{
    if (str.empty() || str[0] < '0' || str[0] > '9') {
        return false;
    }

    int parsed = 0;

    try {
        parsed = std::stoi(str, &last);
    } catch (const std::exception &) {
        return false;
    }

    if (parsed < std::numeric_limits<T>::min() || parsed > std::numeric_limits<T>::max()) {
        return false;
    }

    value = static_cast<T>(parsed);
    return true;
}

endpoint_t* endpoint_t::create(const std::string &url)
{
    const auto proto_delim_pos = url.find("://");
    if (proto_delim_pos == std::string::npos) {
        throw bad_endpoint_t();
    }

    std::string proto = url.substr(0, proto_delim_pos);
    std::string full_address = url.substr(proto_delim_pos + 3);

    if (proto == "udp") {
        return new ipv4_endpoint_t(full_address);
    }
#ifndef NO_CAN_SOCKETS
    else if (proto == "can") {
        return new can_endpoint_t(full_address);
    }
#endif

    throw bad_endpoint_t();
}

ipv4_endpoint_t::ipv4_endpoint_t(const std::string &endpoint)
{
    parse_text(endpoint);
}

void ipv4_endpoint_t::parse_text(const std::string &endpoint)
{
    address_ = 0;
    port_ = 0;

    const auto port_delim_pos = endpoint.find(':');

    if (port_delim_pos == std::string::npos) {
        throw bad_endpoint_t();
    }

    std::string address = endpoint.substr(0, port_delim_pos);
    std::string port_string = endpoint.substr(port_delim_pos + 1);

    // parse address
    for (size_t i = 0; i < 4; ++i) {
        std::uint8_t byte = 0;
        size_t last_index = 0;

        if (!srt_to_number(address, byte, last_index)) {
            throw bad_endpoint_t();
        }

        if (last_index < address.length() && address[last_index] != '.') {
            throw bad_endpoint_t();
        }

        address = last_index >= address.size() ? std::string{} : address.substr(last_index + 1);
        address_ <<= 8;
        address_ += byte;
    }

    if (!address.empty()) {
        throw bad_endpoint_t();
    }

    // parse port
    size_t last_index = 0;
    if (!srt_to_number(port_string, port_, last_index) || last_index != port_string.size()) {
        throw bad_endpoint_t();
    }
}

#ifndef NO_CAN_SOCKETS
can_endpoint_t::can_endpoint_t(const std::string &device)
    : device_(device)
{
    if (device.empty()) {
        throw bad_endpoint_t();
    }
}
#endif
