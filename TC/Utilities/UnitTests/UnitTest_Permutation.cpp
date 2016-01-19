#include "stdafx.h"
#include "UnitTest_Permutation.h"

#include "Utilities\Permutation.h"

#include "Utilities\UtFacility\UnitTestHelpers.h"

using namespace std;

namespace utilities
{
    void UnitTest_Permutation::Run()
    {
        TestFromIndices();
    }

    void UnitTest_Permutation::TestFromIndices()
    {
        vector<int> a{ 0, 1, 2, 3, 4, 5 };
        vector<int> indices{ 1, 3, 5, 4, 0, 2 };
        Permutation::FromIndices(a.begin(), a.end(), indices.cbegin());

        utilities::unit_tests::UnitTestHelper::CheckVectorsForEquality(a, indices);
    }
}
