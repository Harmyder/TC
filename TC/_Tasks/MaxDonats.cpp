#include "stdafx.h"
#include "MaxDonats.h"

using namespace std;
namespace tasks
{
    int RunInternal(const std::vector<int>& donations) {
        array<array<int, 40>, 40> dp;
        const int dCount = (int)donations.size();
        for (int i = 0; i < dCount; ++i) {
            dp[i][i] = donations[i];
            for (int j = i - 1; j >= 0; --j) {
                int localMax = 0;
                localMax = max(localMax, dp[i][j + 1]);
                for (int middle = j + 1; middle < i; ++middle) {
                    localMax = max(localMax, dp[middle - 1][j] + dp[i][middle + 1]);
                }
                localMax = max(localMax, dp[i - 1][j]);
                dp[i][j] = localMax;
            }
        }
        return dp[dCount - 1][0];
    }

    int MaxDonats::Run(vector<int> donations) {
        int first = donations[0];
        donations[0] = 0;
        int resFirst = RunInternal(donations);
        donations[0] = first;
        donations.back() = 0;
        int resLast = RunInternal(donations);
        return max(resFirst, resLast);
    }
}
