#pragma once

#include "Utilities\Hash.h"
#include "SDK\Graph.h"

namespace tasks
{
    class PaliPath
    {
    public:
        PaliPath(const std::vector<int>& a, const std::vector<int>& b, const std::string& c);

        int Run();

    private:
        void StepBfs(
            const std::vector<std::pair< int, int >>& level,
            std::vector<std::pair< int, int >> *next_level,
            int *edges_added,
            bool *has_linked_up);

    private:
        sdk::Graph g_;

        std::vector<std::vector<bool>> dp_explored_;

        std::unordered_map<std::pair<int, int>, char> edges_letters_;
    };
}
