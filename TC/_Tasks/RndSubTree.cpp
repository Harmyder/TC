#include "stdafx.h"

#include "RndSubTree.h"
#include <SDK/Numeric/MR.h>

using namespace std; 

namespace tasks {
    using ll = long long;

    template <class T>
    T mod(T a, T m) {
        a %= m;
        if (a < 0) a += m;
        return a;
    }

    template <class T>
    T inverse(T a, T m) {
        a = a % m;
        if (a == 1) return a;
        if (a < 1) throw;
        return mod((1 - inverse(m, a) * m) / a, m);
    }

    using mr = sdk::numeric::MR<int, ll, 1000000007>;

    template <class T>
    auto fillPascal(int k) {
        vector<vector<T>> c(k + 1, vector<T>(k + 1, 0));
        for (int i = 1; i <= k; ++i) {
            c[i][0] = c[i][i] = 1;
            for (int j = 1; j < i; ++j) {
                c[i][j] = c[i - 1][j] + c[i - 1][j - 1];
            }
        }
        return c;
    }

    template <class T>
    auto fillP2(int k) {
        vector<T> p2(k + 1);
        p2[0] = 1;
        for (int i = 1; i < k + 1; ++i) {
            p2[i] = p2[i - 1] * 2;
        }
        return p2;
    }

    int RndSubTree::Run(int k) {
        if (k == 0 || k == 1) return 0;
        const auto c = fillPascal<mr>(k);
        const auto p2 = fillP2<mr>(k);
        vector<mr> in(k + 1, 0);
        vector<mr> r(k + 1, 0);
        in[0] = in[1]; in[2] = 1;
        r[0] = r[1] = 0; r[2] = 1;
        for (int n = 3; n <= k; ++n) {
            for (int left = 0; left < n; ++left) {
                const auto coef = c[n - 1][left];
                const auto right = n - left - 1;
                const auto in2in = in[left] + in[right];
                const auto root2inter = (mr(right) * r[left] + mr(left) * r[right] + 2 * left*(right));
                const auto inInc = (in2in + root2inter) * coef;
                const auto root2root = r[left] + r[right] + mr(n - 1);
                const auto rInc = root2root * coef;
                in[n] += inInc + rInc;
                r[n] += rInc;
            }
            in[n] /= p2[n - 1];
            r[n] /= p2[n - 1];
        }
        return in[k].get();
    }
}
