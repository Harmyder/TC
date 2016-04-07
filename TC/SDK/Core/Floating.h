#pragma once

#include <cmath>

namespace sdk
{
    bool AlmostEqualAbsolute(float a, float b, float max_absolute_error)
    {
        if (fabs(a - b) < max_absolute_error)
            return true;
        return false;
    }

    bool AlmostEqualRelative(float a, float b, float max_relative_error)
    {
        float relativeError;
        if (fabs(b) > fabs(a))
            relativeError = fabs((a - b) / b);
        else
            relativeError = fabs((a - b) / b);
        if (relativeError <= max_relative_error)
            return true;
        return false;
    }
    
    bool AlmostEqualRelativeOrAbsolute(float a, float b, float max_relative_error, float max_absolute_error)
    {
        if (AlmostEqualAbsolute(a, b, max_absolute_error))
            return true;
        return AlmostEqualRelative(a, b, max_relative_error);
    }
}