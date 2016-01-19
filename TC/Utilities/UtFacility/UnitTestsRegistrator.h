#pragma once

#include "UnitTest.h"

namespace utilities
{
    namespace unit_tests
    {
        class UnitTestsRegistrator
        {
        public:
            void Register();
            void Unregister();

            std::list<UnitTest*> Tests;
        };
    }
}