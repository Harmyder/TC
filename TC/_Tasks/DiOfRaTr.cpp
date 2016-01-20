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

            assert(abs(accumulate(current.begin(), current.end(), 0.f, [](float acc, const pair<int, float>& next) { return acc + next.second; }) - 1.f) < 0.001f);
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

}

namespace tasks
{
    float DiOfRaTr::Run(const vector<int>& a, const vector<int>& b)
    {
        assert(0 == empl(Tree{}));
        assert(1.5 == empl(Tree{ {} }));
        assert(1.75 == empl(Tree{ {}, {} }));
        assert(3.25 == empl(Tree{ { {}, {} }, {} }));
        assert(1.875 == empl(Tree{ {}, {}, {} }));
        assert(3.375 == empl(Tree{ { {} }, { {} } }));
        empl(Tree{ { {}, { { { { {}, {} } }, {} } } }, { { {}, { { {}, {} } } } } });

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
            const int edgesLength = static_cast<int>(2 * (dia_length / 2));

            const Graph::AdjacencyList al = g[center];
            const size_t degree = al.size();

            vector<Tree> singles;
            singles.reserve(degree);
            for (const int v : al)
            {
                Tree t(vector<Tree>{ TreeFactory::FromTreeGraph(g, v, center) });
                singles.push_back(std::move(t));
            }

            map<pair<int, int>, float> possible_lengths;
            for (int i = edgesLength / 2; i <= edgesLength; ++i)
            {
                for (int j = edgesLength / 2; j <= edgesLength; ++j)
                {
                    possible_lengths[{i, j}] = 0.f;
                }
            }

            // First two trees form first hypothesis
            map<int, float> singles0 = empl_internal(singles[0]);
            map<int, float> singles1 = empl_internal(singles[1]);
            for (const auto& s0 : singles0)
            {
                for (const auto& s1 : singles1)
                {
                    possible_lengths.at({ s0.first, s1.first }) = s0.second * s1.second;
                }
            }

            // Tune hypothesis with every next tree
            map<pair<int, int>, float> tmp_possible_lengths = possible_lengths;
            for (int i = 2; i < singles.size(); ++i)
            {
                for_each(tmp_possible_lengths.begin(), tmp_possible_lengths.end(), [](pair<const pair<int, int>, float>& value) { value.second = 0.f; });

                map<int, float> single = empl_internal(singles[i]);
                for (const auto& s : single)
                {
                    for (auto& pl : possible_lengths)
                    {
                        const pair<int, int>& old_lengths = pl.first;
                        pair<int, int> new_lengths = old_lengths;
                        if (old_lengths.first < old_lengths.second)
                        {
                            if (old_lengths.first < s.first)
                                new_lengths = { s.first, old_lengths.second };
                        }
                        else
                        {
                            if (old_lengths.second < s.first)
                                new_lengths = { old_lengths.first, s.first };
                        }

                        tmp_possible_lengths.at(new_lengths) += possible_lengths.at(old_lengths) * s.second;
                    }
                }
                swap(tmp_possible_lengths, possible_lengths);
            }

            res = accumulate(possible_lengths.begin(), possible_lengths.end(), 0.f,
                [](float acc, const pair<pair<int, int>, float>& next)
            { 
                return acc += (next.first.first + next.first.second) * next.second;
            });
        }

        return res;
    }

}
