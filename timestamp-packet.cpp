#include "timestamp-packet.h"

size_t timestamp_packet_t::byte_size() const
{
    return sizeof(std::int64_t);
}

size_t timestamp_packet_t::serialize_to(char *dest, size_t size) const
{
    const auto numeric = std::chrono::duration_cast<resolution_t>(tp_.time_since_epoch()).count();
    static_assert(sizeof(std::int64_t) >= sizeof(numeric), "size mismatch");

    if (size < byte_size()) {
        return 0;
    }

    return pack_int64(numeric, dest);
}

size_t timestamp_packet_t::deserialize(const char *src, size_t size)
{
    if (size < byte_size()) {
        return 0;
    }

    std::int64_t numeric;
    unpack_int64(numeric, src);
    tp_ = std::chrono::system_clock::time_point(resolution_t(numeric));
    return sizeof(std::int64_t);
}
