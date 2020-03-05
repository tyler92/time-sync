#include "socket-option.h"

#include <sys/socket.h>

broadcast_option_t::broadcast_option_t()
    : typed_socket_option_t<int>(SOL_SOCKET, SO_BROADCAST, 1)
{}

reuse_address_option_t::reuse_address_option_t()
    : typed_socket_option_t<int>(SOL_SOCKET, SO_REUSEADDR, 1)
{}
