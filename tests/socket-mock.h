#ifndef SOCKETMOCK_H
#define SOCKETMOCK_H

#include <gmock/gmock.h>
#include "basic-socket.h"

class socket_mock_t : public basic_socket_t
{
public:
    MOCK_CONST_METHOD0(native, int());
    MOCK_METHOD1(bind, void(const endpoint_t &));
    MOCK_METHOD2(recv, size_t(void *, size_t));
    MOCK_METHOD3(sendto, size_t(const endpoint_t &, const void *, size_t));
    MOCK_METHOD1(set_option, void(const socket_option_t &));
};

#endif // SOCKETMOCK_H
