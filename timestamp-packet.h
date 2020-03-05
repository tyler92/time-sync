#ifndef TIMESTAMPPACKET_H
#define TIMESTAMPPACKET_H

#include "net-packet.h"
#include <chrono>

class timestamp_packet_t : public net_packet_t
{
public:
    using resolution_t = std::chrono::milliseconds;

    timestamp_packet_t()
        : tp_(std::chrono::system_clock::now())
    {}

    explicit timestamp_packet_t(const std::chrono::system_clock::time_point &tp)
        : tp_(tp)
    {}

    size_t byte_size() const override;
    size_t serialize_to(char *dest, size_t size) const override;
    size_t deserialize(const char *src, size_t size) override;

    const std::chrono::system_clock::time_point& payload() const
    {
        return tp_;
    }

private:
    std::chrono::system_clock::time_point tp_;
};

#endif // TIMESTAMPPACKET_H
