#pragma once

#include "Utilities\UtFacility\UnitTest.h"

namespace sdk
{
namespace teli
{
    class UnitTest_RbTree final : public utilities::unit_tests::UnitTest
    {
    public:
        void Run() override;

    private:
        void EmptyTree();

        void InsertSize();
		void InsertDuplicate();
        void Begin();
        void End();
    };
}
}
