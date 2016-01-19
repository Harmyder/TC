#pragma once

#include "SDK\Graph.h"

namespace algorithm
{
    class GraphUtils final
    {
    public:
        static std::vector<int> Diameter(const sdk::Graph& graph);
        static std::pair<sdk::Graph, sdk::Graph> CutBridge(const sdk::Graph& connected, sdk::Edge bridge);
    };
}
