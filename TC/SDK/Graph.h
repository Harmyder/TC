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
            AdjacencyList& operator=(const AdjacencyList& other) = delete;

            const std::vector<int>::const_iterator cbegin_;
            const std::vector<int>::const_iterator cend_;

        public:
            AdjacencyList(std::vector<int>::const_iterator cbegin, std::vector<int>::const_iterator cend) :
                cbegin_(cbegin),
                cend_(cend)
            {}

            std::vector<int>::const_iterator begin() const { return cbegin_; }
            std::vector<int>::const_iterator end() const { return cend_; }
            size_t size() const { return cend_ - cbegin_; }
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
        template <class RnIt>
        static void PrepareAB(int elements_count, std::vector<int>::iterator a_begin, std::vector<int>::iterator b_begin, RnIt data_begin);

        // Before call to Construct, a and b must have been prepared
        static Graph Construct(size_t edges_count, std::vector<int>::const_iterator a, std::vector<int>::const_iterator b);
    };

    template <class RnIt>
    void GraphFactory::PrepareAB(int elements_count, std::vector<int>::iterator a_begin, std::vector<int>::iterator b_begin, RnIt data_begin)
    {
        struct Combined
        {
            int a;
            int b;
            RnIt::value_type data;
        };

        std::vector<Combined> combined;
        for (int i = 0; i < elements_count; ++i)
        {
            combined[i] = { a_begin + i, b_begin + i, data_begin + i };
        }

        sort(combined.begin(), combined.end(), [](const Combined& x, const Combined& y) { return (x.a < y.a) && (x.b < y.b); });

        for (int i = 0; i < elements_count; ++i)
        {
            *(a_begin + i) = combined[i].a;
            *(b_begin + i) = combined[i].b;
            *(data_begin + i) = combined[i].data;
        }
    }

}

