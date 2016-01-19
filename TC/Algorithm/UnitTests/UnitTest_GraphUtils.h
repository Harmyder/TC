#pragma once

#include "Utilities\UtFacility\UnitTest.h"

namespace algorithm
{
    class UnitTest_GraphUtils final : public utilities::unit_tests::UnitTest
    {
    public:
        void Run() override;

    private:
        void TestDiameter();
        void TestCutBridge();
    };
}
