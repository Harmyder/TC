#include "stdafx.h"
#include "Graph.h"
#include "Algorithm\BFS.h"
#include "Utilities\Permutation.h"

#include <numeric>

using namespace std;
using namespace algorithm;

namespace sdk
{
    Graph GraphFactory::Construct(size_t edges_count, vector<int>::const_iterator a, vector<int>::const_iterator b)
    {
        vector<int> adjacency_lists;
        adjacency_lists.reserve(edges_count * 2);

        const int vertices_count = max(*max_element(a, a + edges_count), *max_element(b, b + edges_count)) + 1;
        vector<int> vertices_indices(vertices_count + 1, -1);

        for (size_t v = 0; v < vertices_count; ++v)
        {
            vertices_indices[v] = static_cast<int>(adjacency_lists.size());
            for (int i = 0; i < edges_count; ++i)
            {
                if (*(a + i) == v)
                    adjacency_lists.push_back(*(b + i));
                if (*(b + i) == v)
                    adjacency_lists.push_back(*(a + i));
            }
        }

        vertices_indices[vertices_count] = static_cast<int>(adjacency_lists.size()); // sentinel

        assert(find(vertices_indices.begin(), vertices_indices.end(), -1) == vertices_indices.end());

        return{ adjacency_lists, vertices_indices };
    }
}