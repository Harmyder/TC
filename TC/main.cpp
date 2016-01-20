// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <iomanip>
#include <regex>
#include <bitset>
#include <limits>
#include <algorithm>
#include <vector>
#include <type_traits>

#define _USE_MATH_DEFINES
#include <math.h>

#include "_Tasks\DiOfRaTr.h"

#include "Utilities\UtFacility\UnitTestsRegistrator.h"

using namespace std;

using namespace tasks;

void RunUnitTests()
{
    utilities::unit_tests::UnitTestsRegistrator utr;
    utr.Register();

    for (auto &test : utr.Tests)
    {
        (*test).Run();
    }

    utr.Unregister();
}

int main()
{
    RunUnitTests();

    {
        cout << "\tBridge case" << endl;
        cout << DiOfRaTr::Run({ 0, 0, 0, 1, 4 }, { 1, 2, 3, 4, 5 }) << ", expected is " << 6.25 << endl;
        cout << DiOfRaTr::Run({ 0, 0, 0, 1, 2, 3, 5, 6, 8 }, { 1, 2, 3, 4, 5, 6, 7, 8, 9 }) << ", expected is " << 10.53125 << endl;

        cout << "\tCenter vertex case" << endl;
        cout << DiOfRaTr::Run({ 0, 0, 0 }, { 1, 2, 3 }) << ", expected is " << 3.375 << endl;
    }

    cin.get();
    return 0;
}

