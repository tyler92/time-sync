#include <gmock/gmock.h>

#include "broadcast-sender.h"
#include "timestamp-packet.h"
#include "socket-mock.h"

#include "gmock-helpers.h"

using namespace testing;
using namespace std::chrono_literals;

class broadcast_sender_test : public Test
{
public:
    class test_socket_factory_t : public socket_factory_t
    {
    public:
        test_socket_factory_t()
            : socket(new StrictMock<socket_mock_t>())
        {
            unregister_all();
            register_socket(AF_INET, [this]
            {
                return socket;
            });
        }

        StrictMock<socket_mock_t> *socket = nullptr;
    };
};

TEST_F(broadcast_sender_test, send_buffer)
{
    test_socket_factory_t factory;
    EXPECT_CALL(*factory.socket, set_option(_));
    EXPECT_CALL(*factory.socket, sendto(_, "hello", 5));

    broadcast_sender_t sender("udp://127.0.0.1:80", factory);
    sender.send("hello", 5);
}

TEST_F(broadcast_sender_test, send_packet)
{
    test_socket_factory_t factory;
    EXPECT_CALL(*factory.socket, set_option(_));

    const char binary[] = {0, 0, 0, 0, 0, 0, 0, 1};
    EXPECT_CALL(*factory.socket, sendto(_, _, _))
            .With(Args<1, 2>(IsVoidArray(binary, sizeof(binary))))
            .WillOnce(Return(sizeof(binary)));

    broadcast_sender_t sender("udp://127.0.0.1:80", factory);

    timestamp_packet_t packet(std::chrono::system_clock::time_point(1ms));
    ASSERT_EQ(sender.send(packet), packet.byte_size());
}
