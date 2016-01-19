#pragma once

#include "Utilities\UtFacility\UnitTest.h"

namespace algorithm
{
    class UnitTest_BFS final : public utilities::unit_tests::UnitTest
    {
    public:
        void Run() override;

    private:
        void TestChain();
        void TestTree();
    };
}
