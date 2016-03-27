#include "stdafx.h"

#include "SDK\Teli\UnitTests\UnitTest_RbTree.h"

#include "SDK\Teli\RbTree.h"
#include "Utilities\UtFacility\UnitTestHelpers.h"

using namespace std;

using namespace utilities::unit_tests;

namespace sdk
{
namespace teli
{
    void UnitTest_RbTree::Run()
    {
        EmptyTree();
        InsertSize();
		InsertDuplicate();
        Begin();
        End();
    }

    void UnitTest_RbTree::EmptyTree()
    {
        RbTree<int> tree;
        CHECK_TRUE(tree.empty());

        CHECK_EQUAL(tree.size(), (RbTree<int>::size_type)0);

        CHECK_EQUAL(*tree.begin(), *tree.end());
    }

    void UnitTest_RbTree::InsertSize()
    {
        RbTree<int> tree;

        tree.Insert(1);
        CHECK_EQUAL(tree.size(), (RbTree<int>::size_type)1);

        tree.Insert(2);
        CHECK_EQUAL(tree.size(), (RbTree<int>::size_type)2);

		auto iter = tree.cbegin();
		CHECK_EQUAL(*iter, 1);
		CHECK_EQUAL(*++iter, 2);
    }

	void UnitTest_RbTree::InsertDuplicate()
	{
		RbTree<int> tree;

		tree.Insert(1);
		CHECK_EQUAL(tree.size(), (RbTree<int>::size_type)1);

		auto res = tree.Insert(1);
		CHECK_EQUAL(*res.first, 1);
		CHECK_EQUAL(res.second, false);
		CHECK_EQUAL(tree.size(), (RbTree<int>::size_type)1);
	}

    void UnitTest_RbTree::Begin()
    {
        RbTree<int> tree;

        tree.Insert(5);
        CHECK_EQUAL(*tree.begin(), 5);

        tree.Insert(6);
        CHECK_EQUAL(*tree.begin(), 5);

        tree.Insert(4);
        CHECK_EQUAL(*tree.begin(), 4);
    }

    void UnitTest_RbTree::End()
    {
        RbTree<int> tree;

        tree.Insert(5);
        CHECK_EQUAL(*--tree.end(), 5);

        tree.Insert(6);
        CHECK_EQUAL(*--tree.end(), 6);

        tree.Insert(4);
        CHECK_EQUAL(*--tree.end(), 6);
    }

}
}

