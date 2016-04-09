#include "stdafx.h"

#include "AttackOfTheClones.h"

using namespace std;

namespace tasks
{
    int AttackOfTheClones::Run(const vector<int>& a, const vector<int>& b)
    {
        const size_t kN = a.size();

        vector<pair<int, int>> p_start_day;
        p_start_day.reserve(kN);
        for (size_t i = 0; i < kN; ++i)
        {
            p_start_day.push_back({ a[i], (int)i });
        }
        sort(p_start_day.begin(), p_start_day.end(), [](const pair<int, int>& e1, const pair<int, int>& e2) { return e1.first < e2.first; });

        vector<pair<int, int>> p_end_day;
        p_end_day.reserve(kN);
        for (size_t i = 0; i < kN; ++i)
        {
            p_end_day.push_back({ b[i], (int)i });
        }
        sort(p_end_day.begin(), p_end_day.end(), [](const pair<int, int>& e1, const pair<int, int>& e2) { return e1.first < e2.first; });

        int biggest_distance = 0;
        for (size_t i = 0; i < kN; ++i)
        {
            const int distance = p_start_day[i].second - p_end_day[i].second;
            biggest_distance = max(distance, biggest_distance);
        }

        return biggest_distance + 1;
    }
}
