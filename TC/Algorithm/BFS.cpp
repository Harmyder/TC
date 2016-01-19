#include "stdafx.h"
#include "BFS.h"

#include <queue>
using namespace std;

using namespace sdk;

namespace algorithm
{
    vector<BFS::Vertex> BFS::Run(int startIndex)
    {
        std::vector<Vertex> vertices_info(graph_.VerticesCount(), { numeric_limits<int>::max(), -1 });
        std::vector<bool> visited(graph_.VerticesCount(), false);

        struct QVertex
        {
            int index;
            int edge_distance;
            int predecessor;
        };

        struct Comp
        {
            bool operator() (const QVertex& v1, const QVertex& v2) { return v1.edge_distance < v2.edge_distance; }
        };

        priority_queue < QVertex, std::vector<QVertex>, Comp> to_visit;

        to_visit.push({ startIndex, 0, -1 });
        visited[startIndex] = true;

        while (!to_visit.empty())
        {
            const QVertex current = to_visit.top();
            to_visit.pop();

            for (int adjacentIndex : graph_[current.index])
            {
                if (!visited[adjacentIndex])
                {
                    to_visit.push({ adjacentIndex, current.edge_distance + 1, current.index });
                    visited[adjacentIndex] = true;
                }
            }

            vertices_info[current.index].edge_distance = current.edge_distance;
            vertices_info[current.index].predecessor = current.predecessor;
        }
        
        return vertices_info;
    }

}