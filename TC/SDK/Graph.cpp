#include "stdafx.h"
#include "Graph.h"
#include "Algorithm\BFS.h"
#include "Utilities\Permutation.h"

#include <numeric>

using namespace std;
using namespace algorithm;

namespace sdk
{
    void GraphFactory::PrepareAB(vector<int>& a, vector<int>& b)
    {
        class SortIndices
        {
        private:
            const vector<int>& a_;
        public:
            SortIndices(const vector<int>& a) : a_(a) {}
            bool operator()(int i, int j) const { return a_[i] < a_[j]; }
        };

        vector<int> indices(a.size());
        iota(indices.begin(), indices.end(), 0);
        sort(indices.begin(), indices.end());

    }

    Graph GraphFactory::Construct(const vector<int>& a, const vector<int>& b)
    {
        assert(a.size() == b.size());
        const size_t kEdgesCount = a.size();

        vector<int> adjacency_lists;
        adjacency_lists.reserve(kEdgesCount * 2);

        const int vertices_count = max(*max_element(a.begin(), a.end()), *max_element(b.begin(), b.end())) + 1;
        vector<int> vertices_indices(vertices_count + 1, -1);

        for (size_t v = 0; v < vertices_count; ++v)
        {
            vertices_indices[v] = static_cast<int>(adjacency_lists.size());
            for (int i = 0; i < kEdgesCount; ++i)
            {
                if (a[i] == v)
                    adjacency_lists.push_back(b[i]);
                if (b[i] == v)
                    adjacency_lists.push_back(a[i]);
            }
        }

        vertices_indices[vertices_count] = static_cast<int>(adjacency_lists.size()); // sentinel

        assert(find(vertices_indices.begin(), vertices_indices.end(), -1) == vertices_indices.end());

        return{ adjacency_lists, vertices_indices };
    }
}