#include "stdafx.h"

#include "Utilities\UtFacility\UnitTestsRegistrator.h"

#include "Algorithm\UnitTests\UnitTest_BFS.h"
#include "Algorithm\UnitTests\UnitTest_GraphUtils.h"
#include "Algorithm\UnitTests\UnitTest_ConvexHull.h"
#include "Utilities\UnitTests\UnitTest_Permutation.h"
#include "SDK\Teli\UnitTests\UnitTest_RbTree.h"
#include "SDK\Types\UnitTests\UnitTest_Point.h"

using namespace std;
using namespace algorithm;
using namespace sdk;
using namespace teli;

namespace utilities
{
    namespace unit_tests
    {
        void UnitTestsRegistrator::Register()
        {
            Tests.push_back(new UnitTest_BFS());
            Tests.push_back(new UnitTest_GraphUtils());
            Tests.push_back(new UnitTest_Permutation());
            Tests.push_back(new UnitTest_RbTree());
            Tests.push_back(new UnitTest_Point());
            Tests.push_back(new UnitTest_ConvexHull());
        }

        void UnitTestsRegistrator::Unregister()
        {
            for (UnitTest* ut : Tests)
                delete ut;
        }
    }
}