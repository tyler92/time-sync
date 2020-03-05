#include <gtest/gtest.h>

#include "endpoint.h"

class enpoint_parse_test : public testing::Test
{
public:
    struct ipv4_test_t
    {
        std::uint32_t address() const
        {
            std::uint32_t result = 0;

            for (size_t i = 0; i < 4; ++i) {
                result <<= 8;
                result += address_bytes[i];
            }

            return result;
        }

        std::uint8_t address_bytes[4];
        std::uint16_t port;
    };

    struct can_test_t
    {
        std::string device;
    };

    void expect_equal(const endpoint_t &actual,
                      const ipv4_test_t &expected,
                      const std::string &testinfo)
    {
        ASSERT_EQ(actual.family(), AF_INET) << testinfo;

        const auto &ipv4 = static_cast<const ipv4_endpoint_t&>(actual);
        EXPECT_EQ(ipv4.port(), expected.port) << testinfo;
        EXPECT_EQ(ipv4.address(), expected.address()) << testinfo;
    }

#ifndef NO_CAN_SOCKETS
    void expect_equal(const endpoint_t &actual,
                      const can_test_t &expected,
                      const std::string &testinfo)
    {
        ASSERT_EQ(actual.family(), AF_CAN) << testinfo;

        const auto &can = static_cast<const can_endpoint_t&>(actual);
        EXPECT_EQ(can.device(), expected.device) << testinfo;
    }
#endif
};

TEST_F(enpoint_parse_test, parse_udp_correct)
{
    std::pair<std::string, ipv4_test_t> test_cases[] = {
        {"udp://0.0.0.0:1234", {{0, 0, 0, 0}, 1234}},
        {"udp://255.255.255.255:1", {{255, 255, 255, 255}, 1}},
        {"udp://8.8.8.8:65535", {{8, 8, 8, 8}, 65535}},
        {"udp://1.2.3.4:1234", {{1, 2, 3, 4}, 1234}},
    };

    for (const auto &test_case : test_cases) {
        std::unique_ptr<endpoint_t> endpoint(endpoint_t::create(test_case.first));
        expect_equal(*endpoint, test_case.second, test_case.first);
    }
}

TEST_F(enpoint_parse_test, parse_udp_incorrect)
{
    std::string test_cases[] = {
        "udp",
        "0.0.0.0:1234",
        "udp://0.0.0.0",
        "0.0.0.0",
        "udp://256.0.0.0:1234",
        "udp://0.0.0.3000:1234",
        "udp://0.0.0.0:123400",
        "udp://0.0.0.0:abc",
        "udp://a.0.0.0:1234",
        "udp://....:1234",
        "udp://.0.0.0:1234",
        "udp://.0.0.0.0:1234",
        "udp://0.0.0.0.0:1234",
        "udp://0.0.0..0:1234",
        "udp://0.0.0.0:-1234",
        "udp://0.0.0.0:1234a",
        "udp://0.0.0.0: 1234",
    };

    auto invoke = [](const std::string &arg)
    {
        std::unique_ptr<endpoint_t> endpoint(endpoint_t::create(arg));
    };

    for (const auto &test_case : test_cases) {
        EXPECT_THROW(invoke(test_case), bad_endpoint_t) << test_case;
    }
}

#ifndef NO_CAN_SOCKETS
TEST_F(enpoint_parse_test, parse_can_correct)
{
    std::pair<std::string, can_test_t> test_cases[] = {
        {"can://vcan0", {"vcan0"}},
        {"can://vcan1", {"vcan1"}},
        {"can://vcan123", {"vcan123"}},
    };

    for (const auto &test_case : test_cases) {
        std::unique_ptr<endpoint_t> endpoint(endpoint_t::create(test_case.first));
        expect_equal(*endpoint, test_case.second, test_case.first);
    }
}

TEST_F(enpoint_parse_test, parse_can_incorrect)
{
    std::string test_cases[] = {
        "can",
        "can:",
        "can:/",
        "can://",
    };

    auto invoke = [](const std::string &arg)
    {
        std::unique_ptr<endpoint_t> endpoint(endpoint_t::create(arg));
    };

    for (const auto &test_case : test_cases) {
        EXPECT_THROW(invoke(test_case), bad_endpoint_t) << test_case;
    }
}
#endif
