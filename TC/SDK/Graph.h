#pragma once

#include <vector>

namespace sdk
{
    typedef std::pair<int, int> Edge;

    class Graph final
    {
        std::vector<int> vertices_indices_;
        std::vector<int> adjacency_lists_;

    public:
        enum { NO_INDEX = -1 };

        class AdjacencyList
        {
            const std::vector<int>::const_iterator cbegin_;
            const std::vector<int>::const_iterator cend_;

        public:
            AdjacencyList(std::vector<int>::const_iterator cbegin, std::vector<int>::const_iterator cend) :
                cbegin_(cbegin),
                cend_(cend)
            {}

            std::vector<int>::const_iterator begin() const { return cbegin_; }
            std::vector<int>::const_iterator end() const { return cend_; }
        };
        
        // vertices indices must contain sentinel element with total size of adjacency list at the end
        Graph(std::vector<int> adjacencyLists, std::vector<int> verticesIndices) :
            adjacency_lists_(adjacencyLists),
            vertices_indices_(verticesIndices)
        {
            assert(vertices_indices_.back() == adjacency_lists_.size());
        }
        
        size_t VerticesCount() const 
        { 
            return vertices_indices_.size() - 1; // for sentinel
        }

        const AdjacencyList operator[] (int i) const
        { 
            assert(i < vertices_indices_.size());
            return
            { 
                adjacency_lists_.cbegin() + vertices_indices_[i],
                adjacency_lists_.cbegin() + vertices_indices_[i + 1]
            };
        }
    };

    class GraphFactory final
    {
    public:
        static void PrepareAB(std::vector<int>& a, std::vector<int>& b);
        static Graph Construct(const std::vector<int>& a, const std::vector<int>& b);
    };
}

