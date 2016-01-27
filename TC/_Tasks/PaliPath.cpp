#include "stdafx.h"

#include "PaliPath.h"

using namespace std;

using namespace sdk;

namespace tasks
{
    PaliPath::PaliPath(const std::vector<int>& a, const std::vector<int>& b, const std::string& c) :
        g_(GraphFactory::Construct(a.size(), a.cbegin(), b.cbegin()))
    {
        for (size_t i = 0; i < a.size(); ++i)
        {
            edges_letters_[{min(a[i], b[i]), max(a[i], b[i])}] = c[i];
        }
    }

    int PaliPath::Run()
    {
        const size_t vertices_count = g_.VerticesCount();

        dp_explored_.resize(vertices_count, vector<bool>(vertices_count, false));

        vector<pair< int, int >> level{ { 0, 1 } };
        vector<pair< int, int >> level_next;

        int edges_total = 0;
        bool not_done = true;
        while (not_done)
        {
            bool has_linked_up;
            int edges_added;
            StepBfs(level, &level_next, &edges_added, &has_linked_up);
            not_done = !has_linked_up;

            edges_total += edges_added;

            // No more where to go
            if (level_next.size() == 0)
            {
                edges_total = -1;
                break;
            }

            level.swap(level_next);
            level_next.clear();
        }
        

        return edges_total;
    }

    void PaliPath::StepBfs(const vector<pair< int, int >>& level, vector<pair< int, int >> *level_next, int *edges_added, bool *has_linked_up)
    {
        *has_linked_up = false;
        *edges_added = 2;

        for (const auto& l : level)
        {
            int u = l.first;
            int v = l.second;

            if (u == v)
            {
                *edges_added = 0;
                *has_linked_up = true;
                return;
            }

            const auto u_al = g_[u];
            const auto v_al = g_[v];

            for (int u_next : u_al)
            {
                for (int v_next : v_al)
                {
                    // Check if loop has linked up
                    if (u_next == v || u == v_next)
                    {
                        *edges_added = 1;
                        *has_linked_up = true;
                        return;
                    }

                    char u_char = edges_letters_[{min(u, u_next), max(u, u_next)}];
                    char v_char = edges_letters_[{min(v, v_next), max(v, v_next)}];

                    if (u_char == v_char)
                    {
                        // If we have been here already, then we don't need another longer path
                        if (!dp_explored_[u_next][v_next])
                        {
                            dp_explored_[u_next][v_next] = true;

                            level_next->push_back({ u_next, v_next });
                        }
                    }
                }
            }
        }
    }
}
