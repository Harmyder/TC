#pragma once

#include <sstream>

namespace utilities
{
    namespace unit_tests
    {
        class UnitTest
        {
        public:
            UnitTest();

            virtual void Run() = 0;

            static void CheckNotEqual(void* actual, void* expected, char *file, int line);

            static void CheckEqual(const char* actual, const char* expected, char *file, int line);
            static void CheckEqual(const void* actual, const void* expected, char *file, int line);
            static void CheckEqual(int actual, int expected, char *file, int line);
            static void CheckEqual(size_t actual, size_t expected, char *file, int line);
            static void CheckEqual(bool actual, bool expected, char *file, int line);

            template <class T>
            static void CheckEqual(const T& actual, const T& expected, char *file, int line);

            static void CheckTrue(bool value, char *file, int line);
            static void CheckFalse(bool value, char *file, int line);

        private:
            static void ReportFailure(std::string msg, char *file, int line);
        };

        template <class T>
        void UnitTest::CheckEqual(const T& actual, const T& expected, char *file, int line)
        {
            if (actual != expected)
            {
                std::ostringstream ss;
                ss << "\tExpected " << expected << ", but actual is " << actual << "." << std::endl;
                ReportFailure(ss.str(), file, line);
            }
        }

    }
}

#define CHECK_NOT_EQUAL(actual, expected) UnitTest::CheckNotEqual(actual, expected, __FILE__, __LINE__)
#define CHECK_EQUAL(actual, expected) UnitTest::CheckEqual(actual, expected, __FILE__, __LINE__)
#define CHECK_TRUE(value) UnitTest::CheckTrue(value, __FILE__, __LINE__)
#define CHECK_FALSE(value) UnitTest::CheckFalse(value, __FILE__, __LINE__)