#ifndef ENDPOINT_T_H
#define ENDPOINT_T_H

#include <string>
#include <memory>
#include <stdexcept>
#include <sys/socket.h>

class endpoint_t
{
public:
    static endpoint_t* create(const std::string &url);

    virtual int family() const noexcept = 0;

    virtual ~endpoint_t() = default;
};

class ipv4_endpoint_t : public endpoint_t
{
public:
    ipv4_endpoint_t(const std::string &endpoint);

    int family() const noexcept override
    {
        return AF_INET;
    }

    std::uint32_t address() const noexcept
    {
        return address_;
    }

    std::uint16_t port() const noexcept
    {
        return port_;
    }

private:
    void parse_text(const std::string &endpoint);

private:
    std::uint32_t address_;
    std::uint16_t port_;
};

#ifndef NO_CAN_SOCKETS
class can_endpoint_t : public endpoint_t
{
public:
    can_endpoint_t(const std::string &device);

    int family() const noexcept override
    {
        return AF_CAN;
    }

    const char* device() const noexcept
    {
        return device_.c_str();
    }

private:
    std::string device_;
};
#endif

class bad_endpoint_t : public std::runtime_error
{
public:
    bad_endpoint_t()
        : std::runtime_error("bad endpoint")
    {}
};

#endif // ENDPOINT_T_H
