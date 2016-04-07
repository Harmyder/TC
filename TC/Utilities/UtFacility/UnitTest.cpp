#include "stdafx.h"

#include "UnitTest.h"
#include "Utilities\DebugPrint.h"
#include <stdlib.h>

namespace
{
    char* filename(char* path)
    {
        char filename[256];
        char extension[256];

        _splitpath_s(path,
            NULL, 0,
            NULL, 0,
            filename, sizeof(filename),
            extension, sizeof(extension));

        size_t pathlen = strlen(path);
        size_t filenamelen = strnlen_s(filename, sizeof(filename));
        size_t extensionlen = strnlen_s(extension, sizeof(extension));
        return path + pathlen - filenamelen - extensionlen;
    }

    void CheckFailed(char *file, int line)
    {
        utilities::DebugPrintf("%s: %d\n", filename(file), line);
        DebugBreak();
    }
}

namespace utilities
{
    namespace unit_tests
    {
        UnitTest::UnitTest()
        {
        }

        void UnitTest::CheckNotEqual(void* actual, void* expected, char *file, int line)
        {
            if (actual == expected)
            {
                DebugPrintf("\tNot expected %p, but actual is %p.\n", expected, actual);
                CheckFailed(file, line);
            }
        }

        void UnitTest::CheckEqual(const char* actual, const char* expected, char *file, int line)
        {
            if (strcmp(actual, expected))
            {
                DebugPrintf("\tExpected %p, but actual is %p.\n", expected, actual);
                CheckFailed(file, line);
            }
        }

        void UnitTest::CheckEqual(const void* actual, const void* expected, char *file, int line)
        {
            if (actual != expected)
            {
                DebugPrintf("\tExpected %p, but actual is %p.\n", expected, actual);
                CheckFailed(file, line);
            }
        }

        void UnitTest::CheckEqual(int actual, int expected, char *file, int line)
        {
            if (actual != expected)
            {
                DebugPrintf("\tExpected %d, but actual is %d.\n", expected, actual);
                CheckFailed(file, line);
            }
        }
        void UnitTest::CheckEqual(size_t actual, size_t expected, char *file, int line)
        {
            CheckEqual((int)actual, (int)expected, file, line);
        }

        void UnitTest::CheckEqual(bool actual, bool expected, char *file, int line)
        {
            if (actual != expected)
            {
                DebugPrintf("\tExpected %s, but actual is %s.\n", expected ? "true" : "false", actual ? "true" : "false");
                CheckFailed(file, line);
            }
        }

        void UnitTest::CheckTrue(bool value, char *file, int line)
        {
            if (!value)
                ReportFailure("\nTrue expected.\n", file, line);
        }

        void UnitTest::CheckFalse(bool value, char *file, int line)
        {
            if (value)
                ReportFailure("\nFalse expected.\n", file, line);
        }

        void UnitTest::ReportFailure(std::string msg, char *file, int line)
        {
            DebugPrintf(msg.c_str());
            CheckFailed(file, line);
        }
    }
}