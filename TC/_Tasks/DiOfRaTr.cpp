#include "stdafx.h"

#include "DiOfRaTr.h"

#include "SDK\Graph.h"
#include "SDK\Tree.h"
#include "Algorithm\GraphUtils.h"

using namespace std;
using namespace algorithm;

using namespace sdk;

namespace
{
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

    //0     == empl(Tree{})
    //1.5   == empl(Tree{ {} })
    //1.75  == empl(Tree{ {}, {} })
    //3.25  == empl(Tree{ { {}, {} }, {} })
    //1.875 == empl(Tree{ {}, {}, {} })
    //3.375 == empl(Tree{ { {} }, { {} } })
}

namespace tasks
{
    float DiOfRaTr::Run(const vector<int>& a, const vector<int>& b)
    {
        const Graph g = GraphFactory::Construct(a, b);
        const vector<int> dia_path = GraphUtils::Diameter(g);
        const size_t dia_length = dia_path.size();

        float res = 0;
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
            const int center = dia_path[dia_length / 2];

            const Graph::AdjacencyList al = g[center];
            const size_t degree = al.size();

            vector<Tree> singles;
            singles.reserve(degree);
            for (const int v : al)
            {
                singles.push_back(TreeFactory::FromTreeGraph(g, v, center));
            }

            for (int mid = 0; mid < degree; ++mid)
            {
                vector<pair<Tree, Tree>> pairs;
                pairs.reserve(degree);
                for (size_t i = 1; i < degree; ++i)
                {
                    pairs.push_back({
                        Tree(vector<Tree>(singles.begin(), singles.begin() + i)),
                        Tree(vector<Tree>(singles.rbegin(), singles.rbegin() + degree - i))
                    });
                }

                for (size_t i = 0; i < degree - 1; ++i)
                {
                    res = max(res, empl(pairs[i].first) + empl(pairs[i].second));
                }

                rotate(singles.begin(), singles.begin() + 1, singles.end());
            }
        }

        return res;
    }

}
