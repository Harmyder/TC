#include "stdafx.h"

#include "UnitTest_Point.h"

#include <SDK\Types\Point.h>

namespace sdk
{
    void UnitTest_Point::Run()
    {
        TestPseudoangle();
        TestCcw();
    }

    void UnitTest_Point::TestPseudoangle()
    {
        // First quadrant  
        CHECK_TRUE(pseudoangle(Point2<float>(1, 1.5)) > pseudoangle(Point2<float>(1, 1)));
        CHECK_TRUE(pseudoangle(Point2<float>(1, 1)) > pseudoangle(Point2<float>(1, 0.5)));

        // Between quadrants
        CHECK_TRUE(pseudoangle(Point2<int>(1, 1)) > pseudoangle(Point2<int>(1, -1)));
        CHECK_TRUE(pseudoangle(Point2<int>(-1, 1)) > pseudoangle(Point2<int>(1, 1)));
        // Discontinuity at +-pi
        CHECK_TRUE(pseudoangle(Point2<int>(-1, -1)) < pseudoangle(Point2<int>(-1, 1)));
        CHECK_TRUE(pseudoangle(Point2<int>(-1, 1)) > pseudoangle(Point2<int>(1, 1)));
    }

    void UnitTest_Point::TestCcw()
    {
        CHECK_TRUE(ccw(Point2<int>(0, 0), Point2<int>(1, 0), Point2<int>(0, 1)) > 0);
        CHECK_TRUE(ccw(Point2<int>(0, 0), Point2<int>(0, 1), Point2<int>(1, 0)) < 0);

        CHECK_EQUAL(ccw(Point2<int>(0, 0), Point2<int>(0, 0), Point2<int>(1, 1)), 0);
        CHECK_EQUAL(ccw(Point2<int>(0, 0), Point2<int>(1, 1), Point2<int>(2, 2)), 0);
    }
}