#include "stdafx.h"
#include "GraphUtils.h"

#include "Algorithm/BFS.h"

using namespace std;

using namespace sdk;

namespace algorithm
{
    std::vector<int> GraphUtils::Diameter(const Graph& graph)
    {
        BFS bfs(graph);
        const vector<BFS::Vertex> res = bfs.Run(0);

        const auto comp = [](const BFS::Vertex& v1, const BFS::Vertex& v2) { return v1.edge_distance < v2.edge_distance; };
        const int furthest_index = static_cast<int>(abs(distance(res.cbegin(), max_element(res.cbegin(), res.cend(), comp))));
        const vector<BFS::Vertex> res_2 = bfs.Run(furthest_index);

        const int furthest_index_2 = static_cast<int>(abs(distance(res_2.cbegin(), max_element(res_2.cbegin(), res_2.cend(), comp))));

        int index = furthest_index_2;
        vector<int> path;
        path.reserve(res_2[furthest_index_2].edge_distance);
        path.push_back(index);
        while (index != furthest_index)
        {
            index = res_2[index].predecessor;
            path.push_back(index);
        }

        return path;
    }

    pair<Graph, Graph> GraphUtils::CutBridge(const Graph& connected, Edge bridge)
    {
        BFS bfs(connected);
        const int vertices[2] = { bridge.first, bridge.second };

        vector<BFS::Vertex> bfs_tree[2] = { bfs.Run(vertices[0]), bfs.Run(vertices[1]) };

        vector<int> a[2];
        vector<int> b[2];
        for (int i = 0; i < connected.VerticesCount(); ++i)
        {
            const int closest = bfs_tree[0][i].edge_distance < bfs_tree[1][i].edge_distance ? 0 : 1;
            for (int v : connected[i])
            {
                if (i < v) // count every edge only once
                {
                    if (v != 1 - closest)
                    {
                        a[closest].push_back(i);
                        b[closest].push_back(v);
                    }
                    else
                    {
                        assert(i == closest); // ensure bridge
                    }
                }
            }
        }


        return{ GraphFactory::Construct(a[0], b[0]), GraphFactory::Construct(a[1], b[1]) };
    }

}
