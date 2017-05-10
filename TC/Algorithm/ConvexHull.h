#pragma once

#include <vector>
#include <algorithm>

#include <SDK\Types\Point.h>
#include <SDK\Core\Container.h>

namespace sdk
{
    // All methods store output in ccw order
    template <class T>
    class ConvexHull
    {
    public:
        using Point = Point2<T>;
        using const_points_iterator = typename std::vector<Point>::const_iterator;

        static void ComputeNlgN(const_points_iterator begin, const_points_iterator end, std::vector<int> *hull);

        // Output-sensitive algorithms which operates in O(n*lg(h)).
        static void ComputeNlgH(const_points_iterator begin, const_points_iterator end, std::vector<int> *hull);

        // Binary search over convex polygon
        // If ran on convex polygon, comp must have only one transition from 0 to 1 end one from 1 to 0
        // Thus, expected output would be 00000011111111 or it also can be 1111000001111, but it can't be 0101
        template <class const_iterator, class Comp>
        static const_iterator FindMax(const_iterator begin, const_iterator end, Comp comp);

    private:
        static void GrachamScan(const_points_iterator begin, const_points_iterator end, std::vector<int> *hull);

        static void ChanAlgorithm(const_points_iterator begin, const_points_iterator end, std::vector<int> *hull);
        static void ChanAlgorithmInternal(const_points_iterator begin, const_points_iterator end, int m, int h, std::vector<int> *hull);
    };

    template <class T>
    void ConvexHull<T>::ComputeNlgN(const_points_iterator begin, const_points_iterator end, std::vector<int> *hull)
    {
        const int points_count = static_cast<int>(end - begin);

        // Degeneracies
        switch (points_count)
        {
        case 0:
            return;
        case 1:
            hull->push_back(0);
            return;
        case 2:
            hull->push_back(0);
            hull->push_back(1);
            return;
        }

        return GrachamScan(begin, end, hull);
    }

    template <class T>
    void ConvexHull<T>::GrachamScan(const_points_iterator begin, const_points_iterator end, std::vector<int> *hull)
    {
        const int points_count = static_cast<int>(end - begin);

        using PointWithIndex = std::pair<int, const Point*>;
        std::vector<PointWithIndex> p_w_i;
        p_w_i.reserve(points_count);
        for (int i = 0; i < points_count; ++i)
        {
            p_w_i.push_back(std::make_pair(i, &*(begin+i)));
        }

        // 1. Find point with lowest y-coordinate
        auto min_y = std::min_element(p_w_i.begin(), p_w_i.end(), [](const PointWithIndex& p1, const PointWithIndex& p2) { return p1.second->y() < p2.second->y(); });
        std::swap(*p_w_i.begin(), *min_y);

        // 2. Sort by polar angle with first point, the one with minimum y-coordinate
        const Point& min_y_point = *p_w_i.begin()->second;
        std::sort(
            p_w_i.begin() + 1,
            p_w_i.end(),
            [&min_y_point](const PointWithIndex& p1, const PointWithIndex& p2) { return pseudoangle(*p1.second - min_y_point) < pseudoangle(*p2.second - min_y_point); }
        );

        // 3. Build convex hull
        int hull_size = 2;
        for (int i = 2; i < points_count; ++i)
        {
            // While not ccw
            while (ccw(*p_w_i[hull_size - 2].second, *p_w_i[hull_size - 1].second, *p_w_i[i].second) <= 0)
            {
                if (hull_size > 2)
                {
                    --hull_size;
                }
                else if (hull_size == 2)
                {
                    swap(p_w_i[1], p_w_i[i]);
                    i++;
                }
                
                if (i == points_count)
                {
                    break;
                }
            }

            if (i != points_count)
            {
                swap(p_w_i[hull_size], p_w_i[i]);
                ++hull_size;
            }
        }

        // 4. Fill output vector
        if (hull->capacity() - hull->size() < (size_t)hull_size)
            hull->reserve(hull->size() + hull_size);

        for (int i = 0; i < hull_size; ++i)
        {
            hull->push_back(p_w_i[i].first);
        }
    }

    template <class T>
    void ConvexHull<T>::ComputeNlgH(const_points_iterator begin, const_points_iterator end, std::vector<int> *hull)
    {
        const int points_count = static_cast<int>(end - begin);

        // Degeneracies
        switch (points_count)
        {
        case 0:
            return;
        case 1:
            hull->push_back(0);
            return;
        case 2:
            hull->push_back(0);
            hull->push_back(1);
            return;
        }

        if (points_count < 20)
        {
            ComputeNlgN(begin, end, hull);
            return;
        }

        ChanAlgorithm(begin, end, hull);
    }

    template <class T>
    void ConvexHull<T>::ChanAlgorithm(const_points_iterator begin, const_points_iterator end, std::vector<int> *hull)
    {
        const int points_count = static_cast<int>(end - begin);
        const auto initial_hull_size = hull->size();

        for (int t = 1; ; ++t)
        {
            const int m = std::min(static_cast<int>(std::pow(4, t)), points_count);
            const int h = m;

            ChanAlgorithmInternal(begin, end, m, h, hull);

            if (hull->size() != initial_hull_size)
            {
                return;
            }
        }
    }

    template <class T>
    void ConvexHull<T>::ChanAlgorithmInternal(const_points_iterator begin, const_points_iterator end, int m, int h, std::vector<int> *hull)
    {
        std::vector<const_points_iterator> begins;
        std::vector<const_points_iterator> ends;

        const int points_count = static_cast<int>(end - begin);
        const int subsets_count = static_cast<int>(std::ceil(static_cast<float>(points_count) / m));
        begins.reserve(subsets_count);
        ends.reserve(subsets_count);

        for (int i = 0; i < subsets_count - 1; i++)
        {
            begins.push_back(begin + m * i);
            ends.push_back(begin + m * (i + 1));
        }
        begins.push_back(begin + m * (subsets_count - 1));
        ends.push_back(end);

        const Point kStartPoint(0, std::numeric_limits<T>::lowest());
        std::vector<int> tentative_hull;
        tentative_hull.push_back(-1);

        // Insert the rightmost point
        tentative_hull.push_back(0);
        for (int i = 1; i < points_count; ++i)
        {
            if ((begin + tentative_hull.back())->x() < (begin + i)->x())
                tentative_hull.back() = i;
        }

        std::pair<int, int> base_hull_point; // hull index, entry index
        std::vector<std::vector<int>> hulls(subsets_count);
        for (int i = 0; i < subsets_count; i++)
        {
            ComputeNlgN(begins[i], ends[i], &hulls[i]);
            for (int j = 0; j < static_cast<int>(hulls[i].size()); ++j)
            {
                hulls[i][j] += i * m; 
                if (hulls[i][j] == tentative_hull.back())
                {
                    base_hull_point = { i, j };
                }
            }
        }

        for (int k = 1; k <= h; ++k)
        {
            const Point& base = *(begin + tentative_hull[k]);
            const Point& leg = k == 1 ? kStartPoint : *(begin + tentative_hull[k - 1]);
            const Point& first_ray = leg - base;
            const double base_angle = pseudoangle(first_ray);
            auto comp = [&kStartPoint, &base, &base_angle, &begin](int i1, int i2) {
                if (i1 == i2)
                    return false;
                const Point& p1 = i1 == -1 ? kStartPoint : *(begin + i1);
                const Point& p2 = *(begin + i2);
                if (p1 == base)
                    return true;
                if (p2 == base)
                    return false;
                double p1_angle = pseudoangle(p1 - base);
                double p2_angle = pseudoangle(p2 - base);

                // Both between -pi and base_angle
                if (p1_angle < base_angle && p2_angle < base_angle)
                    return p1_angle > p2_angle;
                // Both between base_angle and +pi
                if (p1_angle > base_angle && p2_angle > base_angle)
                    return p1_angle > p2_angle;
                if (p1_angle == base_angle)
                    return true;
                if (p2_angle == base_angle)
                    return false;
                // p1_angle is bigger
                if (p1_angle < base_angle)
                    return true;
                // p2_angle is bigger
                if (p2_angle < base_angle)
                    return false;
                // (p1_angle == base_angle && p2_angle == base_angle)
                throw;
            };
            tentative_hull.push_back(tentative_hull[k - 1]);
            std::pair<int, int> new_base_hull;
            for (int i = 0; i < subsets_count; i++)
            {
                int q_index;

                // Search for tangent line for this hull from the base point
                if (base_hull_point.first == i)
                {
                    // If base point is in current hull, then tangent can be only along one of its incident edges
                    const int entry_index = base_hull_point.second;
                    const int entry_index_plus = sdk::wrap_around(hulls[i].size(), entry_index + 1);
                    const int entry_index_minus = sdk::wrap_around(hulls[i].size(), entry_index - 1);
                    q_index = comp(hulls[i][entry_index_minus], hulls[i][entry_index_plus]) ? entry_index_plus : entry_index_minus;
                }
                else
                    q_index = static_cast<int>(FindMax(hulls[i].cbegin(), hulls[i].cend(), comp) - hulls[i].cbegin());

                if (comp(tentative_hull[k + 1], hulls[i][q_index]))
                {
                    new_base_hull = { i, q_index };
                    tentative_hull[k + 1] = hulls[i][q_index];
                }
            }
            base_hull_point = new_base_hull;

            if (tentative_hull[k + 1] == tentative_hull[1])
            {
                const auto start = hull->size();
                hull->resize(start + k);
                std::copy(tentative_hull.cbegin() + 1, tentative_hull.cend() - 1, hull->begin() + start);
                break;
            }
        }
    }

    template <class T>
    template <class const_iterator, class Comp>
    const_iterator ConvexHull<T>::FindMax(const_iterator begin, const_iterator end, Comp comp)
    {
        if (end - begin < 12)
        {
            return std::max_element(begin, end, comp);
        }

        auto middle = begin + (end - begin) / 2;
        auto middle_plus = middle + 1;
        auto middle_minus = middle - 1;

        bool go_up = comp(*middle, *middle_plus);
        bool go_down = comp(*middle, *middle_minus);

        if (!go_up && !go_down)
            return middle;
        if (go_up)
            return FindMax(middle, end, comp);
        if (go_down)
            return FindMax(begin, middle, comp);
        
        return begin;
    }
}
