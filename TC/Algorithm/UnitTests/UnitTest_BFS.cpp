#include "stdafx.h"

#include "UnitTest_BFS.h"

#include "Algorithm\BFS.h"
#include "SDK\Graph.h"
#include "Utilities\UtFacility\UnitTestHelpers.h"

using namespace std;

using namespace algorithm;
using namespace sdk;

using namespace utilities::unit_tests;

namespace
{
    void SingleTest(const vector<int>& a, const vector<int>& b, const vector<BFS::Vertex> expected)
    {
        Graph g = GraphFactory::Construct(a, b);
        BFS bfs(g);
        const vector<BFS::Vertex> actual = bfs.Run(0);

        CHECK_EQUAL(actual.size(), expected.size());
        if (actual.size() == expected.size())
        {
            for (size_t i = 0; i < actual.size(); i++)
            {
                CHECK_EQUAL(actual[i].edge_distance, expected[i].edge_distance);
                CHECK_EQUAL(actual[i].predecessor, expected[i].predecessor);
            }
        }
    }
}

namespace algorithm
{
    void UnitTest_BFS::Run()
    {
        TestChain();
        TestTree();
    }

    void UnitTest_BFS::TestChain()
    {
        SingleTest({ 0, 1, 2, 3 }, { 1, 2, 3, 4 }, { { 0, -1 }, { 1, 0 }, { 2, 1 }, { 3, 2 }, { 4, 3 } });
    }

    void UnitTest_BFS::TestTree()
    {
        SingleTest({ 0, 0, 0, 1, 1, 2 }, { 1, 2, 3, 4, 5, 6 }, { { 0, -1 }, { 1, 0 }, { 1, 0 }, { 1, 0 }, { 2, 1 }, { 2, 1 }, { 2, 2 } });
    }
}
