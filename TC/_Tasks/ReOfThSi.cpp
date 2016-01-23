#include "stdafx.h"

#include "ReOfThSi.h"

using namespace std;

namespace
{
    int Loss(int step_height, int safe_height)
    {
        int sr_loss = max(step_height - safe_height, 0);
        return sr_loss * sr_loss;
    }
}

namespace tasks
{
    int& ReOfThSi::Subtask(size_t bottom, size_t top, size_t moveable)
    {
        const size_t plates = top - bottom - 1;
        assert(moveable <= plates);

        return dp_[bottom][top][moveable];
    }

    // Not really DP, because when we cane to a subtask with all moveable or non-moveable, we compute instead of recurse deeper
    int ReOfThSi::Run(const std::vector<int>& steps, int T, int D)
    {
        assert(T < steps.size());

        const size_t stepsCount = steps.size();

        dp_ = vector<vector<vector<int>>>(stepsCount, vector<vector<int>>(stepsCount + 1, vector<int>(T + 1, numeric_limits<int>::max())));

        for (int moveable = 0; moveable <= T; ++moveable)
        {
            for (int k = 1; k <= stepsCount; ++k)
            {
                for (int bottom = 0; bottom <= stepsCount - k; ++bottom)
                {
                    int top = bottom + k;
                    {
                        if (moveable < k)
                        {
                            int penalty = 0;

                            // None moveable
                            if (moveable == 0)
                            {
                                penalty = accumulate(steps.cbegin() + bottom, steps.cbegin() + top, 0, [D](int a, int step){ return a + Loss(step, D); });
                            }
                            // All moveable
                            else if (moveable == k - 1)
                            {
                                int span = accumulate(steps.begin() + bottom, steps.begin() + top, 0);
                                const int optimal_difference = span / k;
                                const int optimal_difference_one = optimal_difference + 1;
                                const int left_out = span % optimal_difference;

                                penalty
                                    = left_out * Loss(optimal_difference_one, D)
                                    + (k - left_out) * Loss(optimal_difference, D);
                            }
                            // Some moveable, some not
                            else
                            {
                                assert(moveable < k - 1);

                                penalty = numeric_limits<int>::max();
                                for (int p = 1; p < k; ++p)
                                {
                                    const int overall_plates = k - 1;
                                    const int left_plates = p - 1;
                                    const int right_plates = overall_plates - left_plates - 1;
                                    vector<int> c;
                                    for (int i = 0; i <= moveable; ++i)
                                    {
                                        if (i <= left_plates && moveable - i <= right_plates)
                                            c.push_back(Subtask(bottom, bottom + p, i) + Subtask(bottom + p, top, moveable - i));
                                    }
                                    penalty = min(penalty, *min(c.cbegin(), c.cend()));
                                }
                            }

                            Subtask(bottom, top, moveable) = penalty;
                        }
                    }
                }
            }
        }

        return Subtask(0, stepsCount, T);
    }
}
