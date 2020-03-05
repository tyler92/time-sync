#ifndef TIMEPRINTER_H
#define TIMEPRINTER_H

#include <ostream>
#include <chrono>

namespace details
{
    template <class T>
    struct time_unit_string
    {};

    std::ostream& operator<<(std::ostream &os, time_unit_string<std::chrono::nanoseconds>)
    {
        return os << "ns";
    }

    std::ostream& operator<<(std::ostream &os, time_unit_string<std::chrono::microseconds>)
    {
        return os << "us";
    }

    std::ostream& operator<<(std::ostream &os, time_unit_string<std::chrono::milliseconds>)
    {
        return os << "ms";
    }

    std::ostream& operator<<(std::ostream &os, time_unit_string<std::chrono::seconds>)
    {
        return os << "s";
    }
}

class time_printer_t
{
public:
    template <class T, class R = std::chrono::milliseconds>
    void print_to(std::ostream &os, const T &server_time, const T &client_time)
    {
        const auto &diff_time = server_time - client_time;

        using namespace std::chrono;
        os << "server "
           << duration_cast<R>(server_time.time_since_epoch()).count()
           << ", client "
           << duration_cast<R>(client_time.time_since_epoch()).count()
           << ", diff "
           << duration_cast<R>(diff_time).count() << ' '
           << details::time_unit_string<R>{}
           << std::endl;
    }
};

#endif // TIMEPRINTER_H
