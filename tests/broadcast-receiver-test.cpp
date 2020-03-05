#include <gmock/gmock.h>

#include "broadcast-receiver.h"
#include "timestamp-packet.h"
#include "socket-mock.h"

#include <netinet/in.h>

using namespace testing;
using namespace std::chrono_literals;

class broadcast_receiver_test : public Test
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

TEST_F(broadcast_receiver_test, receive_buffer)
{
    test_socket_factory_t factory;
    EXPECT_CALL(*factory.socket, set_option(_));
    EXPECT_CALL(*factory.socket, bind(_));
    EXPECT_CALL(*factory.socket, recv(_, _)).WillOnce(Invoke([](void *buf, size_t len)
    {
        if (len >= 5) {
            std::memcpy(buf, "hello", 5);
            return 5;
        }

        return 0;
    }));

    broadcast_receiver_t sender("udp://127.0.0.1:80", factory);

    std::string buffer(5, 0);
    ASSERT_EQ(sender.receive(&buffer[0], buffer.size()), buffer.size());
    ASSERT_EQ(buffer, "hello");
}

TEST_F(broadcast_receiver_test, receive_packet)
{
    test_socket_factory_t factory;
    EXPECT_CALL(*factory.socket, set_option(_));
    EXPECT_CALL(*factory.socket, bind(_));
    EXPECT_CALL(*factory.socket, recv(_, _)).WillOnce(Invoke([](void *buf, size_t len)
    {
        if (len >= 8) {
            const char binary[] = {0, 0, 0, 0, 0, 0, 0, 1};
            std::memcpy(buf, binary, sizeof(binary));
            return 8;
        }

        return 8;
    }));

    broadcast_receiver_t sender("udp://127.0.0.1:80", factory);

    timestamp_packet_t packet;
    ASSERT_EQ(sender.receive(packet), packet.byte_size());
    ASSERT_EQ(packet.payload().time_since_epoch(), 1ms);
}
