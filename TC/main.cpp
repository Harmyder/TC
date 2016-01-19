// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <iomanip>
#include <regex>
#include <bitset>
#include <limits>
#include <algorithm>
#include <vector>
#include <map>
#include <type_traits>

#define _USE_MATH_DEFINES
#include <math.h>

#include "SDK\Tree.h"
#include "Algorithm\BFS.h"
#include "Algorithm\GraphUtils.h"

#include "Utilities\UtFacility\UnitTestsRegistrator.h"

using namespace std;

void RunUnitTests()
{
    utilities::unit_tests::UnitTestsRegistrator utr;
    utr.Register();

    for (auto &test : utr.Tests)
    {
        (*test).Run();
    }

    utr.Unregister();
}

using namespace sdk;
using namespace algorithm;

const int EdgeFirst = 1;
const int EdgeSecond = 2;

map<int, float> empl_internal(const Tree &t)
{
    map<int, float> prev;
    prev[0] = 1.f;

    for (const auto& c : t.GetChildren())
    {
        map<int, float> current;

        for (auto& lpFirst : prev)
        {
            for (auto& lpSecond : empl_internal(c))
            {
                const bool firstPath = prev[0] == 1;

                if (firstPath)
                {
                    const float probability = lpSecond.second * 0.5f; // we will add an edge
                    current[lpSecond.first + EdgeFirst] += probability;
                    current[lpSecond.first + EdgeSecond] += probability;
                }
                else
                {
                    const float probability = lpFirst.second * lpSecond.second * 0.5f; // we will add an edge for right tree

                    const int l1 = max(lpFirst.first, lpSecond.first + EdgeFirst);
                    current[l1] += probability;

                    const int l2 = max(lpFirst.first, lpSecond.first + EdgeSecond);
                    current[l2] += probability;
                }
            }
        }

        swap(prev, current);
    }

    return prev;
}

float empl(const Tree &t)
{
    float expected = 0;
    for (const auto& a : empl_internal(t))
    {
        expected += a.first * a.second;
    }

    return expected;
}

int main()
{
    int myints[] = { 10, 20, 30, 5, 15 };
    std::vector<int> v(myints, myints + 5);

    std::make_heap(v.begin(), v.end());

    RunUnitTests();


    const vector<int> a{ 0, 0, 0 };
    const vector<int> b{ 1, 2, 3 };
    const Graph g = GraphFactory::Construct(a, b);
    const vector<int> dia_path = GraphUtils::Diameter(g);
    const size_t dia_length = dia_path.size();

    float res;
    if (dia_length % 2 == 0)
    {
        // There is an edge in the middle, it is a bridge
        Edge bridge = { dia_path[dia_length / 2 - 1], dia_path[dia_length / 2] };
        Tree t1 = TreeFactory::FromTreeGraph(g, bridge.first, bridge.second);
        Tree t2 = TreeFactory::FromTreeGraph(g, bridge.second, bridge.first);
        res = empl(t1) + 1.5f + empl(t2);
    }
    else
    {
        // There is a vertex in the middle

    }

    cout << empl(Tree{}) << endl;
    cout << empl(Tree{ {} }) << endl;
    cout << empl(Tree{ {}, {} }) << endl;
    cout << empl(Tree{ { {}, {} }, {} }) << endl;
    cout << empl(Tree{ {}, {}, {} }) << endl;
    cout << empl(Tree{ { {} }, { {} } }) << endl;

    cin.get();
    return 0;
}

