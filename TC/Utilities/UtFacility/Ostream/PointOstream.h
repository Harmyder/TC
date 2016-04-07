#pragma once

#include <SDK/Types/Point.h>

#include <iostream>

namespace
{
    template <class T>
    std::ostream& operator<<(std::ostream& os, const sdk::Point2<T>& p)
    {
        os << p.x() << " " << p.y();
        return os;
    }
}
