#ifndef GMOCKHELPERS_H
#define GMOCKHELPERS_H

#include <gmock/gmock.h>

template <class T>
testing::Matcher<std::tuple<const void*, size_t>> IsVoidArray(const T *ptr, size_t size)
{
    using namespace testing;

    class TupleConverter : public std::tuple<const T*, size_t>
    {
    public:
        TupleConverter(const std::tuple<const void*, size_t> &t)
            : std::tuple<const T*, size_t>(static_cast<const T*>(std::get<0>(t)), std::get<1>(t))
        {}
    };

    return SafeMatcherCast<std::tuple<const void*, size_t>>(
        SafeMatcherCast<TupleConverter>(
            SafeMatcherCast<std::tuple<const T*, size_t>>(
                ElementsAreArray(ptr, size)))
    );
}

#endif // GMOCKHELPERS_H
