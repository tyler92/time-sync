#ifndef SOCKETOPTION_H
#define SOCKETOPTION_H

#include "endpoint.h"
#include <vector>
#include <cstring>
#include <type_traits>

class socket_option_t
{
public:
    socket_option_t(int level, int name, const std::vector<char> &value)
        : level_(level)
        , name_(name)
        , value_(value)
    {}

    int level() const { return level_; }
    int name() const { return name_; }
    const std::vector<char>& value() const { return value_; }

private:
    int level_;
    int name_;
    std::vector<char> value_;
};

template <class T>
class typed_socket_option_t : public socket_option_t
{
public:
    typed_socket_option_t(int level, int name, const T &value)
        : socket_option_t(level, name, make_binary(value))
    {}

private:
    static std::vector<char> make_binary(const T &value)
    {
        static_assert(std::is_pod<T>::value, "T must be POD");

        std::vector<char> buffer(sizeof(T));
        std::memcpy(buffer.data(), &value, sizeof(T));
        return buffer;
    }
};

class broadcast_option_t : public typed_socket_option_t<int>
{
public:
    broadcast_option_t();
};

class reuse_address_option_t : public typed_socket_option_t<int>
{
public:
    reuse_address_option_t();
};

#endif // SOCKETOPTION_H
