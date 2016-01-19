#pragma once

#include "SDK\Graph.h"

namespace algorithm
{
    class BFS final
    {
    public:
        BFS(const sdk::Graph& graph) : graph_(graph) {}

        struct Vertex
        {
            int edge_distance;
            int predecessor;
        };

        std::vector<Vertex> Run(int startIndex);

    private:
        const sdk::Graph& graph_;
    };
}

