#pragma once

#include "Utilities\UtFacility\UnitTest.h"

namespace utilities
{
    class UnitTest_Permutation final : public utilities::unit_tests::UnitTest
    {
    public:
        void Run() override;

    private:
        void TestFromIndices();
    };
}

