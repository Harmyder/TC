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

    template <class T1, class T2>
    std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& p) {
        os << "pair(" << p.first << ", " << p.second << ")" << std::endl;
        return os;
    }
}
