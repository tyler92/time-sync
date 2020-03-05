#include <gtest/gtest.h>

#include "time-printer.h"
#include <sstream>

using time_point = std::chrono::system_clock::time_point;
using namespace std::chrono_literals;

class time_printer_test : public testing::Test
{
public:
    struct test_case_t
    {
        std::chrono::milliseconds server;
        std::chrono::milliseconds client;
        std::chrono::milliseconds expected_diff;
    };

    std::string expected_output(const test_case_t &test_case)
    {
        return "server " + std::to_string(test_case.server.count()) +
                ", client " + std::to_string(test_case.client.count()) +
                ", diff " + std::to_string(test_case.expected_diff.count()) +
                " ms\n";
    }

    void check(const test_case_t &test_case)
    {
        time_printer_t printer;
        std::ostringstream oss;

        printer.print_to(oss, time_point(test_case.server), time_point(test_case.client));
        EXPECT_EQ(oss.str(), expected_output(test_case));
    }
};

TEST_F(time_printer_test, print)
{
    test_case_t test_cases[] = {
        {0s, 0s, 0s},
        {1s, 0s, 1s},
        {0s, 1s, -1s},
        {1s, 1s, 0s},
        {1s, 1ms, 999ms},
        {100ms, 1ms, 99ms},
        {1583320020910ms, 1583320020909ms, 1ms},
    };

    for (const auto &test_case : test_cases) {
        check(test_case);
    }
}
