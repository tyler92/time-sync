#ifndef POSIXERRORCHECK_H
#define POSIXERRORCHECK_H

#include <cstring>
#include <system_error>
#include <iostream>

template <class T, class D>
T check_error(T rc, D &&description, error_t error = errno)
{
    if (rc == -1) {
        throw std::system_error(error, std::system_category(), std::forward<D>(description));
    }

    return rc;
}

#endif // POSIXERRORCHECK_H
