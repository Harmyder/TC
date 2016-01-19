#include "stdafx.h"
#include "Tree.h"
#include "Graph.h"

using namespace std;

namespace sdk
{
    Tree TreeFactory::FromTreeGraph(const Graph& g, int root, int no_follow)
    {
        vector<Tree> children;
        for (int v : g[root])
        {
            if (v != no_follow)
            {
                children.push_back(FromTreeGraph(g, v, root));
            }
        }

        return Tree(children);
    }
}