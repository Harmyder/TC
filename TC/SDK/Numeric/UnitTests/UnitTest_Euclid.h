#pragma once

#include "Utilities\UtFacility\UnitTest.h"

namespace sdk
{
    namespace numeric
    {
        class UnitTest_Euclid final : public utilities::unit_tests::UnitTest
        {
        public:
            void Run() override;

        private:
            void gcd0();
            void gcdEqual();
            void gcdBigLL();
            void gcdOrder();

            void extendedEuclidConcrete();
            void extendedEuclidSwap0();
            void invMod0();
            void invModConcrete();

            void chineseConcrete();
        };
    }
}
