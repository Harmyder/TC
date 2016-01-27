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
#include "_Tasks\ReOfThSi.h"
#include "_Tasks\PaliPath.h"

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
        cout << PaliPath({ 2, 2, 0, 3, 4 }, { 0, 1, 3, 4, 1 }, "abxyx").Run() << ", expected is " << 3 << endl;
        cout << PaliPath({ 0, 0, 3, 4, 5, 6 }, { 2, 3, 4, 5, 6, 1 }, "abaaaa").Run() << ", expected is " << 9 << endl;
        cout << PaliPath({ 2, 2, 0, 3, 4 }, { 0, 1, 3, 4, 1 }, "abxyz").Run() << ", expected is " << -1 << endl;
        cout << PaliPath({ 0, 0, 3, 4, 5 }, { 2, 3, 4, 5, 1 }, "abaaa").Run() << ", expected is " << -1 << endl;
        cout << PaliPath({ 0 }, { 1 }, "x").Run() << ", expected is " << -1 << endl;
    }

    {
        cout << endl << "DiOfRaTr:" << endl;

        cout << "\tBridge case" << endl;
        cout << DiOfRaTr::Run({ 0, 0, 0, 1, 4 }, { 1, 2, 3, 4, 5 }) << ", expected is " << 6.25 << endl;
        cout << DiOfRaTr::Run({ 0, 0, 0, 1, 2, 3, 5, 6, 8 }, { 1, 2, 3, 4, 5, 6, 7, 8, 9 }) << ", expected is " << 10.53125 << endl;

        cout << "\tCenter vertex case" << endl;
        cout << DiOfRaTr::Run({ 0, 0, 0 }, { 1, 2, 3 }) << ", expected is " << 3.375 << endl;
    }

    {
        cout << endl << "ReOfThSi:" << endl;
        ReOfThSi rots;
        cout << rots.Run({ 1, 1, 1, 1, 1, 1, 1 }, 3, 3) << " expected is " << 0 << endl;
        cout << rots.Run({ 1, 2, 3, 4, 5, 6 }, 1, 2) << " expected is " << 30 << endl;
        cout << rots.Run({ 2, 3, 5 }, 1, 1) << " expected is " << 19 << endl;
        cout << rots.Run({ 2, 3, 5 }, 2, 1) << " expected is " << 17 << endl;
        cout << rots.Run({ 1, 2, 3 }, 2, 2) << " expected is " << 0 << endl;
    }

    cin.get();
    return 0;
}

