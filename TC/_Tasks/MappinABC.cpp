#include "stdafx.h"
#include "MappinABC.h"

using namespace std;

namespace tasks
{
    constexpr int kMaxPositions = 3000 + 1;
    constexpr int kMaxElement = 3000 + 1;

    int bits_cnt(int mask) {
        int count = 0;
        while (mask > 0) {
            if ((mask & 1) == 1) count++;
            mask >>= 1;
        }
        return count;
    }

    enum Mask {
        None = 0,
        A = 1,
        B = 2,
        AB =  A | B,
        C = 4,
        AC = A | C,
        BC = B | C,
        ABC = A | B | C,
        Count = ABC + 1,
    };
    Mask operator&(Mask a, Mask b) { return Mask(underlying_type_t<Mask>(a) & underlying_type_t<Mask>(b)); }
    Mask& operator&=(Mask& a, Mask b) { a = a & b; return a; }

    int MappinABC::Run(const vector<int>& t) {
        constexpr int mod = 1000000007;
        const int N = (int)t.size();
        int occurs[kMaxElement] = {};
        for (auto s : t) ++occurs[s];

        int pmask[(int)Mask::Count][kMaxPositions];
        for (int m = 0; m < Mask::Count; ++m) {
            pmask[m][0] = 1;
            const int bc = bits_cnt(m);
            for (int j = 1; j < kMaxPositions; ++j) pmask[m][j] = (long long)(pmask[m][j - 1]) * bc % mod;
        }

        long long ret = 0;
        for (int a = 0; a < N; ++a) {
            int out[Mask::Count] = {};
            int in[Mask::Count] = {};
            Mask masks[kMaxElement] = {};
            int aoccurs[kMaxPositions] = {};
            for (int i = 0; i < kMaxElement; ++i) masks[i] = Mask::ABC;

            for (int i = 0; i < a; ++i) { masks[t[i]] = Mask::BC; ++aoccurs[t[i]]; }
            if (aoccurs[t[a]] != 0) continue;
            masks[t[a]] = Mask::A; ++aoccurs[t[a]];

            for (int i = 0; i < kMaxElement; ++i) if (occurs[i] != 0) {
                if (aoccurs[i] == occurs[i]) ++out[masks[i]];
                else ++in[masks[i]];
            }

            for (int c = N - 1; c > a + 1; --c) {
                const int tc = t[c];
                --in[masks[tc]];
                if (masks[tc] & Mask::C) {
                    long long ret_out = 1;
                    long long ret_in = 1;
                    long long ret_in_ex = 1;
                    for (int m = 1; m < Mask::Count; ++m) {
                        ret_out = ret_out * pmask[m][out[m]] % mod;
                        ret_in = ret_in * pmask[m][in[m]] % mod;
                        ret_in_ex = ret_in_ex * pmask[m & Mask::AC][in[m]] % mod;
                    }
                    ret += ret_out * (ret_in - ret_in_ex) % mod;
                    masks[tc] &= Mask::AB;
                }

                ++aoccurs[tc];
                if (aoccurs[tc] == occurs[tc]) ++out[masks[tc]];
                else ++in[masks[tc]];
            }
        }

        return ret % mod;
    }
}
