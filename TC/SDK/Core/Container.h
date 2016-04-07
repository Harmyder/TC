#pragma once

#include <cmath>

namespace sdk
{
    template <class S, class I>
    I wrap_around(S size, I index)
    {
        if (index == -1)
            return static_cast<I>(size - 1);
        if (index == size)
            return static_cast<I>(0);
        return index;
    }
}