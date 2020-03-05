#include <gtest/gtest.h>

#include "timestamp-packet.h"

using namespace std::chrono_literals;
using time_point = std::chrono::system_clock::time_point;

class timestamp_packet_test : public testing::Test
{};

TEST_F(timestamp_packet_test, byte_size)
{
    timestamp_packet_t packet;
    ASSERT_EQ(packet.byte_size(), 8);
}

TEST_F(timestamp_packet_test, serialize)
{
    time_point timepoints[] = {
        time_point(0ms),
        time_point(257ms),
        time_point(1025ms),
        time_point(65599ms),
        time_point(1583320020910ms),
        time_point(-1583320020910ms),
    };

    for (const auto &tp : timepoints) {
        timestamp_packet_t out_packet(tp);

        const auto binary = out_packet.serialize();
        ASSERT_EQ(binary.size(), out_packet.byte_size());

        timestamp_packet_t in_packet;
        ASSERT_EQ(in_packet.deserialize(binary.data(), binary.size()), binary.size());
        ASSERT_EQ(in_packet.payload(), tp);
    }
}

TEST_F(timestamp_packet_test, various_input_size)
{
    timestamp_packet_t packet;
    std::vector<char> binary(packet.byte_size() * 2);

    for (size_t i = 0; i < packet.byte_size(); ++i) {
        ASSERT_EQ(packet.deserialize(binary.data(), i), 0);
    }

    for (size_t i = packet.byte_size(); i < binary.size(); ++i) {
        ASSERT_EQ(packet.deserialize(binary.data(), i), packet.byte_size());
    }
}
