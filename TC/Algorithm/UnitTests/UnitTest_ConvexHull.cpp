#include "stdafx.h"

#include "UnitTest_ConvexHull.h"

#include "Utilities\UtFacility\UnitTestHelpers.h"

#include <Algorithm\ConvexHull.h>
#include <SDK\Core\Floating.h>
#include <SDK\Types\Point.h>

#include <iterator>

using namespace sdk;

namespace
{
    template <class Point>
    void fill_circle(int vertices_count, typename Point::value_type radius, std::back_insert_iterator<std::vector<Point>> back_inserter)
    {
        const double kPi = std::atan(1.0) * 4;
        for (int i = 0; i < vertices_count; ++i)
        {
            const double rad = i * 2 * kPi / vertices_count;
            back_inserter = Point(static_cast<Point::value_type>(std::cos(rad)), static_cast<Point::value_type>(std::sin(rad))) * radius;
        }
    }
}

namespace algorithm
{
    void UnitTest_ConvexHull::Run()
    {
        TestCompute();
        TestFindMax();
    }

    void UnitTest_ConvexHull::TestFindMax()
    {
        // Test with empty hull
        {
            using Point = Point2<int>;
            const std::vector<Point> convex_hull{};
            auto res = ConvexHull<int>::FindMax(convex_hull.cbegin(), convex_hull.cend(), [](const Point& p1, const Point& p2) { return p1.y() < p2.y(); });
            CHECK_TRUE(res == convex_hull.cend());
        }

        // Test with small hull
        {
            using Point = Point2<int>;
            const std::vector<Point> convex_hull{ Point(0,0), Point(1,0), Point(2,0), Point(1,1) };
            auto res = ConvexHull<int>::FindMax(convex_hull.cbegin(), convex_hull.cend(), [](const Point& p1, const Point& p2) { return p1.y() < p2.y(); });
            const Point expected(1, 1);
            CHECK_EQUAL(*res, expected);
        }

        // Test with large hull
        {
            using Point = Point2<float>;
            std::vector<Point> convex_hull;
            const int vertices_count = 60;
            convex_hull.reserve(vertices_count);
            fill_circle(vertices_count, 1.f, std::back_inserter(convex_hull));

            const int shift_size = 10;
            const int iters_count = vertices_count / shift_size - 1;
            for (int i = 0; i < iters_count; ++i)
            {
                auto res = ConvexHull<float>::FindMax(convex_hull.cbegin(), convex_hull.cend(), [](const Point& p1, const Point& p2) { return p1.y() < p2.y(); });
                CHECK_TRUE(AlmostEqualAbsolute(res->x(), 0, 0.00001f));
                CHECK_TRUE(AlmostEqualRelative(res->y(), 1, 0.00001f));
                std::rotate(convex_hull.begin(), convex_hull.begin() + shift_size, convex_hull.end());
            }
        }
    }

    void UnitTest_ConvexHull::TestCompute()
    {
        // Test with empty vector
        {
            std::vector<Point2<int>> points;
            std::vector<int> hull;
            ConvexHull<int>::ComputeNlgH(points.cbegin(), points.cend(), &hull);
            CHECK_EQUAL((int)hull.size(), 0);
        }

        // Test with all points in line
        {
            using Point = Point2<int>;
            std::vector<Point> points{ Point(0,1), Point(0,2), Point(0,3), Point(0,4) };
            std::vector<int> hull;
            ConvexHull<int>::ComputeNlgH(points.cbegin(), points.cend(), &hull);
            CHECK_EQUAL((int)hull.size(), 2);
            CHECK_EQUAL(hull[0], 0);
            CHECK_EQUAL(hull.back(), (int)points.size() - 1);
        }

        // Test circle
        {
            using Point = Point2<float>;
            std::vector<Point> points;
            const int vertices_count = 60;
            points.reserve(vertices_count);
            fill_circle(vertices_count, 1.f, std::back_inserter(points));

            std::vector<int> hull;
            ConvexHull<float>::ComputeNlgH(points.cbegin(), points.cend(), &hull);

            CHECK_EQUAL((int)hull.size(), vertices_count);
            bool zero_index_met = false;
            for (int i = 1; i < vertices_count; ++i)
            {
                if (hull[i - 1] == 0)
                    zero_index_met = true;
                CHECK_TRUE(hull[i] == hull[i - 1] + 1 || !zero_index_met && hull[i] == 0);
            }
        }

        // Test circle with circle inside
        {
            using Point = Point2<float>;
            std::vector<Point> points;
            const int vertices_count = 60;
            points.reserve(vertices_count * 2);
            fill_circle(vertices_count, 2.f, std::back_inserter(points));
            fill_circle(vertices_count, 1.f, std::back_inserter(points));

            std::vector<int> hull;
            ConvexHull<float>::ComputeNlgH(points.cbegin(), points.cend(), &hull);

            std::vector<int> expected_hull(vertices_count);
            std::iota(expected_hull.begin(), expected_hull.end(), 0);
            bool zero_index_met = false;
            for (int i = 1; i < vertices_count; ++i)
            {
                if (hull[i - 1] == 0)
                    zero_index_met = true;
                CHECK_TRUE(hull[i] < 60 && hull[i] == hull[i - 1] + 1 || !zero_index_met && hull[i] == 0);
            }
        }

        // Rotated wedding rings
        {
            using Point = Point2<float>;
            std::vector<Point> points;
            const int vertices_count = 60;
            points.reserve(vertices_count * 2);
            fill_circle(vertices_count, 2.f, std::back_inserter(points));
            fill_circle(vertices_count, 2.f, std::back_inserter(points));
            // Move second circle along y axis a bit
            std::transform(points.begin() + vertices_count, points.end(), points.begin() + vertices_count, [](const Point& p) { return p + Point(0.f, 1.f); });

            std::vector<int> hull;
            ConvexHull<float>::ComputeNlgH(points.cbegin(), points.cend(), &hull);

            std::vector<int> expected_hull(vertices_count + 2);
            expected_hull[0] = 0; // Rightmost vertex of the lowest circle
            std::iota(expected_hull.begin() + 1, expected_hull.begin() + 32, 60);
            std::iota(expected_hull.begin() + 32, expected_hull.end(), 30);
            utilities::unit_tests::UnitTestHelper::CheckVectorsForEquality(hull, expected_hull);
        }
    }
}