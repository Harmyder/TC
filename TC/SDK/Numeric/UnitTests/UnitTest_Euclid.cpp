#include "stdafx.h"

#include "SDK\Numeric\UnitTests\UnitTest_Euclid.h"

#include "SDK\Numeric\Euclid.h"

#include <limits>

using namespace std;

using namespace utilities::unit_tests;

namespace sdk
{
    namespace numeric
    {
        void UnitTest_Euclid::Run() {
            gcd0();
            gcdEqual();
            gcdBigLL();
            gcdOrder();

            extendedEuclidSwap0();
            extendedEuclidConcrete();
            invMod0();
            invModConcrete();

            chineseConcrete();
        }

        void UnitTest_Euclid::gcd0() {
            CHECK_EQUAL(gcd(0, 0), 0);
        }

        void UnitTest_Euclid::gcdEqual() {
            const int a = 1;
            CHECK_EQUAL(gcd(a, a), a);
        }

        void UnitTest_Euclid::gcdBigLL() {
            using ULL = unsigned long long;
            const ULL a = numeric_limits<ULL>::max() &~(ULL)1;
            const ULL b = a / 2;
            CHECK_EQUAL(gcd(a, b), b);
        }

        void UnitTest_Euclid::gcdOrder() {
            const int a = 100;
            const int b = 80;
            CHECK_EQUAL(gcd(a, b), gcd(b, a));
        }

        void UnitTest_Euclid::extendedEuclidSwap0() {
            CHECK_EQUAL(extendedEuclid(0, 1), make_pair(1, make_pair(0, 1)));
        }

        void UnitTest_Euclid::extendedEuclidConcrete() {
            CHECK_EQUAL(extendedEuclid(7, 13), make_pair(1, make_pair(2, -1)));
        }

        void UnitTest_Euclid::invMod0() {
            CHECK_EQUAL(invMod(1, 0), 1);
        }

        void UnitTest_Euclid::invModConcrete() {
            CHECK_EQUAL(invMod(3, 26), 9);
        }

        void UnitTest_Euclid::chineseConcrete() {
            array<int, 3> mods = { 3, 5, 7 };
            array<int, 3> rems = { 2, 3, 5 };
            CHECK_EQUAL(chinese(mods.cbegin(), mods.cend(), rems.cbegin()), 68);
        }
    }
}

