#pragma once

#include <utility>
#include <algorithm>
using namespace std;

namespace sdk {
namespace numeric {

    namespace detail {
        template <class T> T gcdInternal(T a, T b) {
            while (b != 0) {
                T t = b;
                b = a % b;
                a = t;
            }
            return a;
        }

        template <class T> std::pair<T, std::pair<T, T>> extendedEuclidInternal(T a, T b) {
            T old_x = 1;
            T old_y = 0;
            T x = 0;
            T y = 1;
            while (true) {
                T q = a / b;
                T t = a - b * q;
                if (t == 0) {
                    break;
                }
                a = b;
                b = t;
                t = x; x = old_x - x * q; old_x = t;
                t = y; y = old_y - y * q; old_y = t;
            }
            return make_pair(b, make_pair(x, y));
        }
    }
    
    template <class T> T gcd(T a, T b) {
        if (a > b)
            return detail::gcdInternal(a - b, b);
        return detail::gcdInternal(a, b - a);
    }

    // Returns gcd and Bezout's coefficients
    template <class T> std::pair<T, std::pair<T, T>> extendedEuclid(T a, T b) {
        if (a > b) {
            if (b == 0) return make_pair(a, make_pair(1, 0));
            return detail::extendedEuclidInternal(a, b);
        }
        else {
            if (a == 0) return make_pair(b, make_pair(0, 1));
            auto p = detail::extendedEuclidInternal(b, a);
            swap(p.second.first, p.second.second);
            return p;
        }
    }

    template <class T> T invMod(T a, T m) {
        auto p = extendedEuclid(a, m);
        assert(p.first == 1);
        return p.second.first;
    }

    template <class It> typename It::value_type chinese(It modsB, It modsE, It remsB) {
        const auto big = accumulate(next(modsB), modsE, *modsB, std::multiplies<It::value_type>());
        It::value_type answer = 0;
        for (auto m = modsB; m < modsE; ++m, ++remsB) {
            auto n = big / *m; // multiplying and dividing by mod might adversely affect accuracy and cause SO in accumulate
            answer += (*remsB) * (n * invMod(n, *m));
        }
        answer = answer % big;
        if (answer < 0) answer = big + answer;
        return answer;
    }
}
}

