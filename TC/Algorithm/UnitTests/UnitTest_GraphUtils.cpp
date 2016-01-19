#include "stdafx.h"
#include "UnitTest_GraphUtils.h"

#include "Algorithm\GraphUtils.h"
#include "SDK\Graph.h"

#include "Utilities\UtFacility\UnitTestHelpers.h"

using namespace std;

using namespace sdk;
using namespace algorithm;

using namespace utilities::unit_tests;

namespace algorithm
{
    void UnitTest_GraphUtils::Run()
    {
        TestDiameter();
        TestCutBridge();
    }

    void UnitTest_GraphUtils::TestDiameter()
    {
        //             0
        //            /|\
        //           1 2 3
        //          /| |
        //         4 5 6 
        Graph g = GraphFactory::Construct({ 0, 0, 0, 1, 1, 2 }, { 1, 2, 3, 4, 5, 6 });
        const auto path = GraphUtils::Diameter(g);
        
        const int expected_diameter = 4;
        CHECK_EQUAL((int)path.size() - 1, expected_diameter);
    }

    void UnitTest_GraphUtils::TestCutBridge()
    {
        Graph g = GraphFactory::Construct({ 0, 0, 0, 1, 1, 2 }, { 1, 2, 3, 4, 5, 6 });
        const auto p = GraphUtils::CutBridge(g, { 0, 1 });
    }
}