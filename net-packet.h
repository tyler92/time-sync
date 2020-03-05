#ifndef NETPACKET_H
#define NETPACKET_H

#include <cstddef>
#include <cstdint>
#include <vector>

class net_packet_t
{
public:
    virtual size_t byte_size() const = 0;
    virtual size_t serialize_to(char *data, size_t size) const = 0;
    virtual size_t deserialize(const char *data, size_t size) = 0;

    std::vector<char> serialize() const;

protected:
    size_t pack_int64(std::int64_t n, char *dest) const;
    size_t unpack_int64(std::int64_t &n, const char *dest) const;
};

#endif // NETPACKET_H
