cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 14)

project(time-sync)

option(BUILD_TESTS No "Build unit tests")
option(NO_CAN_SOCKETS No "Build without CAN sockets support")

if (NOT ${NO_CAN_SOCKETS})
    set(CAN_SOCKETS_SRC can-socket.cpp can-socket.h)
endif()

add_library(common-lib STATIC
    endpoint.cpp endpoint.h
    basic-socket.h
    socket-option.cpp socket-option.h
    udp-socket.cpp udp-socket.h
    socket-factory.cpp socket-factory.h
    net-packet.cpp net-packet.h
    timestamp-packet.cpp timestamp-packet.h
    broadcast-sender.cpp broadcast-sender.h
    broadcast-receiver.cpp broadcast-receiver.h
    time-printer.h
    posix-error-check.h
    ${CAN_SOCKETS_SRC}
)

add_executable(time-sync-server
    server.cpp
)

target_link_libraries(time-sync-server
    common-lib
)

add_executable(time-sync-client
    client.cpp
)

target_link_libraries(time-sync-client
    common-lib
)

if (${NO_CAN_SOCKETS})
    target_compile_definitions(common-lib PUBLIC -DNO_CAN_SOCKETS)
endif()

if (BUILD_TESTS)
    add_subdirectory(tests)
endif()
