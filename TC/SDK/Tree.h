#pragma once

namespace sdk
{
    class Tree final
    {
    public:
        Tree() {}
        Tree(std::initializer_list<Tree> children) : children_(children) {}
        Tree(std::vector<Tree> children) : children_(children) {}

        const std::vector<Tree>& GetChildren() const { return children_; }

    private:
        std::vector<Tree> children_;
    };

    class Graph;

    class TreeFactory final
    {
    public:
        static Tree FromTreeGraph(const Graph& tree_graph, int root, int no_follow);
    };
}