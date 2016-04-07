#pragma once

#include "Utilities\UtFacility\UnitTest.h"

namespace algorithm
{
    class UnitTest_ConvexHull final : public utilities::unit_tests::UnitTest
    {
    public:
        void Run() override;

    private:
        void TestFindMax();
        void TestCompute();
    };
}

