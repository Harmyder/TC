#include "BearEmptyCoin.h"

#include <algorithm>

using ll = long long;

namespace tasks {
    int gcd(int x, int y) {
        return x ? gcd(y % x, x) : y;
    }

    long long BearEmptyCoin::win_probability(int K, int S) {
        int i, j;

        if (S < 0) S = -S;

        if (S % K == 0) return (1ll << K);

        bool can[61];
        for (i = 1; i <= K; i++) can[i] = (S % gcd(K, i) == 0);

        ll C[61][61];
        for (i = 0; i < K + 1; ++i) {
            for (j = 0; j < i + 1; ++j) {
                if (j == 0 || j == i) C[i][j] = 1;
                else C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
            }
        }

        ll ans = 0;
        if (can[K]) ans += 2;

        for (i = 1; i < K; ++i) {
            long long best = 0;
            for (j = 0; j <= K - i - 1; ++j)
                if (can[1 + j]) best = std::max(best,  2 * C[K - i - 1][j]);
            ans += best;
        }

        return ans;
    }
}
