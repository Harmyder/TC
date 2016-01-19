#include "stdafx.h"

#include "Utilities\UtFacility\UnitTestsRegistrator.h"

#include "Algorithm\UnitTests\UnitTest_BFS.h"
#include "Algorithm\UnitTests\UnitTest_GraphUtils.h"
#include "Utilities\UnitTests\UnitTest_Permutation.h"

using namespace std;
using namespace algorithm;

namespace utilities
{
    namespace unit_tests
    {
        void UnitTestsRegistrator::Register()
        {
            Tests.push_back(new UnitTest_BFS());
            Tests.push_back(new UnitTest_GraphUtils());
            Tests.push_back(new UnitTest_Permutation());
        }

        void UnitTestsRegistrator::Unregister()
        {
            for (UnitTest* ut : Tests)
                delete ut;
        }
    }
}