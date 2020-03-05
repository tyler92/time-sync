#include "net-packet.h"

#include <netinet/in.h>
#include <cstring>

std::vector<char> net_packet_t::serialize() const
{
    std::vector<char> buf(byte_size());
    serialize_to(buf.data(), buf.size());
    return buf;
}

size_t net_packet_t::pack_int64(std::int64_t n, char *dest) const
{
    std::uint32_t hi = htonl(n >> 32);
    std::uint32_t lo = htonl(n & 0xFFFFFFFF);
    std::memcpy(dest, &hi, sizeof(std::uint32_t));
    std::memcpy(dest + sizeof(std::uint32_t), &lo, sizeof(std::uint32_t));
    return sizeof(std::int64_t);
}

size_t net_packet_t::unpack_int64(std::int64_t &n, const char *dest) const
{
    std::uint32_t hi, lo;
    std::memcpy(&hi, dest, sizeof(std::uint32_t));
    std::memcpy(&lo, dest + sizeof(std::uint32_t), sizeof(std::uint32_t));
    n = ntohl(hi);
    n <<= 32;
    n += ntohl(lo);
    return sizeof(std::int64_t);
}
