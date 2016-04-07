#pragma once

#include "Utilities\UtFacility\UnitTest.h"

namespace sdk
{
    class UnitTest_Point final : public utilities::unit_tests::UnitTest
    {
    public:
        void Run() override;

    private:
        void TestPseudoangle();
        void TestCcw();
    };
}
