// -*- m_testv2.h -*-
// Copyright (C) 2020 Ashish Bailkeri
// SPDX-License-Identifier: MIT
//
// History:
// 0.11 added complete cross compatibility for both Windows, Linux, and MacOS console output
// 0.10 full code-refactor. Instead of using forced #if statements use a class, StdOut write messages
// 0.09 run valgrind to fix invalid reading const char * values. Switched first and last to std::string
// 0.08 switched from use of regular Diagnostic to shared_ptr diagnostic
// 0.07 run tests to fix memory issues
// 0.06 add suite to the test running method
// 0.05 use std::map for adding testing suites
// 0.04 added "AddStuff" in order to add diagnostics and functions to the unit system
// 0.03 add windows pathway and linux pathway for cross-platform use
// 0.02 setup error system
// 0.01 setup basic diagnostics
//
//
//    http://www.opensource.org/licenses/mit-license.php
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#   ifndef MINT_M_TESTV2_H
#   define MINT_M_TESTV2_H
#   if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined(__WIN32) && !defined(__CYGWIN__)
#       define Windows true
#       include <windows.h>
#   elif __APPLE__
#       define Apple true
#       include <TargetConditionals.h>
#   elif __linux__ || __unix__
#       define Linux true
#       include <unistd.h>
#   else
#       error "Could Not Find A Valid OS"
#   endif
#   if defined(__GNUC__) || defined(__clang__) || defined(__psp2__)
#       define __should_inline__    inline
#       define __force_inline__    inline __attribute__((always_inline))
#   elif defined(_MSC_VER)
#       define __should_inline__    __inline
#       define __force_inline__    __forceinline
#   endif
#   if defined(Windows)
#       define COLOR_WHITE     0
#       define COLOR_RED       4
#       define COLOR_GREEN     2
#       define COLOR_YELLOW    14
#       define COLOR_BLUE      9
#       define COLOR_CYAN      3
#       define COLOR_RESET     7
#   elif defined(Linux) || defined(Apple)
#       define COLOR_WHITE     "\x1B[1;37m"
#       define COLOR_RED       "\x1B[1;31m"
#       define COLOR_GREEN     "\x1B[1;32m"
#       define COLOR_YELLOW    "\x1B[1;33m"
#       define COLOR_BLUE      "\x1B[1;34m"
#       define COLOR_CYAN      "\x1B[1;36m"
#       define COLOR_RESET     "\x1B[0m"
#   else
#       error "Could Not Find A Valid OS"
#   endif
#   define __MTest(F) F // Function decoration for Mint Test functions
#   include <map>
#   include <vector>
#   include <utility>
#   include <iostream>
#   include <memory>
#   include <sstream>

// --------------- HELPERS --------------- //
namespace StdOut
{
#if defined(Linux) || defined(Mac)
    static constexpr auto WHITE     = COLOR_WHITE;
    static constexpr auto RED       = COLOR_RED;
    static constexpr auto GREEN     = COLOR_GREEN;
    static constexpr auto YELLOW    = COLOR_YELLOW;
    static constexpr auto BLUE      = COLOR_BLUE;
    static constexpr auto CYAN      = COLOR_CYAN;
    static constexpr auto RESET     = COLOR_RESET;
#elif defined(Windows)

    enum ConsoleColors {
        WHITE  = COLOR_WHITE,
        RED    = COLOR_RED,
        GREEN  = COLOR_GREEN,
        YELLOW = COLOR_YELLOW,
        BLUE   = COLOR_BLUE,
        CYAN   = COLOR_CYAN,
        RESET  = COLOR_RESET
    };

#endif

#if defined(Windows)
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    template<typename Arg>
    static auto Write2(Arg value) {
        if constexpr (std::is_enum<Arg>::value == std::is_enum<ConsoleColors>::value) {
            SetConsoleTextAttribute(console, value);
        } else {
            std::cout << value;
        }
    }

#endif
    template<typename ... Args>
    __should_inline__ static auto __MTest(Write)(Args ... args)
    {
#if defined(Linux) || defined(Mac)
        ((std::cout << args), ...); // Open a fold expression
#elif defined(Windows)
        ((Write2(args)), ...);
#endif
    }
}


// --------------- MAIN CODE --------------- //

typedef void (*MFunc)(); // Alias for void functions

class Diagnostic
{
public:
    Diagnostic() = default;
    template<class First, class Second> // @NOTE: the generic types are for the expected values on assertion error
    Diagnostic(First first, Second second, const char *error, const char *file, const char *str, int line, bool expect)
            : error(error), file(file), str(str), line(line), expect(expect)
    {
        std::stringstream v1; // Create a string stream to convert the two expected variable types
        v1 << first;  // Run a conversion using the "<<" operator, see C++ doc for more info

        // Populate the value into a c_str because we do not need the extra stuff from std::string
        auto converter_1 = v1.str();
        this->first      = converter_1;

        std::stringstream v2; // Create a string stream to convert the two expected variable types
        v2 << second;  // Run a conversion using the "<<" operator, see C++ doc for more info

        // Populate the value into a c_str because we do not need the extra stuff from std::string
        auto converter_2 = v2.str();
        this->second     = converter_2;
    }
    const char *error   = "";     // Assertion or Check Error
    const char *file    = "";     // File on which error occurred
    const char *str     = "";     // The string representation of the call
    std::string first;            // First Value
    std::string second;           // Second Value
    int         line    = 1;      // Line on which error occurred
    bool expect = false; // Special Error Message
};

struct MTestFunction
{
    MTestFunction(const char *name, MFunc function) : name(name), function(function) {}
    const char *name;       // Function name (used in suite and diagnostics)
    MFunc       function;   // Recorded function to evaluate
};

struct MTestSuite
{
    MTestSuite() = default;
    MTestSuite(const char *name) : name(name) {}
    const char                  *name;
    std::vector<MTestFunction>  functions;
};

class MUnit
{
private:
    std::vector<MTestSuite>    suites;          // All Testing Suites
    std::vector<Diagnostic> diagnostics;        // All Diagnostics
    int                     passed  = 0;         // Number of Passed Tests
    int                     failed  = 0;         // Number of Failed Tests
    char                    stop    = false;     // Assertion Failure Stop
    char                    w_error = false;     // Write Out Failed Check
public:
    template<class First, class Second>
    __should_inline__ void __MTest(AddDiagnostic)(First first, Second second, const char *FILE, const char *str, int LINE, const char *error, bool assert, bool expect = false)
    {
        Diagnostic d = Diagnostic(first, second, error, FILE, str, LINE, expect);
        diagnostics.emplace_back(d);
        this->stop    = assert;
        this->w_error = !assert;
        this->failed++;
    }

    void __MTest(AddTestSuite)(const char *SUITE, const char *FUNC_NAME, MFunc function)
    {
        // Create Function
        MTestFunction func  = MTestFunction(FUNC_NAME, function);

        static std::map<const char *, size_t> su;
        if (su.find(SUITE) == su.end())
        {
            // Create Suite
            MTestSuite suite = MTestSuite(SUITE);
            suite.functions.emplace_back(func);

            // Add To Map
            su.emplace(SUITE, suites.size());

            // Add To Vector
            suites.emplace_back(suite);
        }
        else
        {
            auto pos = su.at(SUITE);
            (suites[pos]).functions.emplace_back(func);
        }
    }

    static void __MTest(PrintErrorMessage)(size_t iter, MTestFunction function, size_t functions, Diagnostic *error)
    {
        StdOut::Write(StdOut::CYAN, "[", iter, "/", functions, "]", StdOut::GREEN, " Testing: ", StdOut::BLUE, function.name, StdOut::WHITE, " ..............", StdOut::RED, " Failure ", StdOut::RESET, "\n");
        StdOut::Write(StdOut::WHITE, error->file, ":", error->line, ":", 4, ": ", StdOut::RED, "error: ", StdOut::WHITE, error->error, "\n", StdOut::RESET);

        if (error->expect)
        {
            StdOut::Write(StdOut::BLUE, " Expected : ", StdOut::GREEN, error->second, "\n", StdOut::RESET);
            StdOut::Write(StdOut::BLUE, " Got      : ", StdOut::GREEN, error->first, StdOut::RESET, "\n\n");
        }
        else
        {
            StdOut::Write(StdOut::BLUE, " V1: ", StdOut::GREEN, error->first, "\n");
            StdOut::Write(StdOut::BLUE, " V2: ", StdOut::RED, error->second, StdOut::RESET, "\n\n");
        }
        auto line_num_str = std::to_string(error->line);
        StdOut::Write("  ", error->line - 1, "   |\n");
        StdOut::Write(StdOut::RED, "> ", line_num_str, "   | ", "     ", error->str, StdOut::RESET, "\n");
        StdOut::Write("  ", error->line + 1, "   |");

        size_t loop = line_num_str.size() + 3;
        for (size_t j = 0; j <= loop; ++j)
            StdOut::Write(StdOut::RED, "~");
        StdOut::Write("^", StdOut::RESET, "\n");
    }

    static void __MTest(PrintTestingAnimation)(size_t iter, MTestFunction function, size_t functions)
    {
        bool sw = false;
        int  it = 0;
        while (1)
        {
            it++;
            StdOut::Write(StdOut::CYAN, "[", iter, "/", functions, "]", StdOut::GREEN, " Testing: ", StdOut::BLUE, function.name, " ", StdOut::RESET);
            for (size_t n = 1; n <= 3; ++n)
            {
                sw ? std::cout << " " : std::cout << "." << std::flush;
#if defined(Windows)
                Sleep(100);
#elif defined(Linux) || defined(Apple)
                nanosleep((const struct timespec[]){ 0, 100000000L }, NULL);
#endif
            }
            StdOut::Write("\r");
            sw = !sw;
            if (it == 4) break;
        }
    }

    void __MTest(RunTests)(void)
    {
        size_t r = 0;
        for (; r < suites.size(); ++r)
        {
            // Number of functions
            size_t functions = suites[r].functions.size();

            // Write out initial running statement
            StdOut::Write(StdOut::GREEN, "-------------------------\n");
            StdOut::Write(StdOut::BLUE, " Running Suite: ", StdOut::YELLOW, suites[r].name, "\n");
            StdOut::Write(StdOut::GREEN, "-------------------------", StdOut::RESET, "\n", StdOut::RESET);

            for (size_t j = 0; j < functions; ++j)
            {
                auto next     = j + 1;
                auto diag     = diagnostics.size();
                auto test_fn  = suites[r].functions[j];
                auto function = test_fn.function;
                function();


                if (stop || w_error)
                {
                    PrintTestingAnimation(next, test_fn, functions);
                    PrintErrorMessage(next, test_fn, functions, &diagnostics[diagnostics.size() - 1]);

                    if (stop)
                    {
                        stop = false;
                        break;
                    } else
                        w_error = false;
                }


                if (diag == diagnostics.size())
                {
                    PrintTestingAnimation(next, test_fn, functions);
                    StdOut::Write(StdOut::CYAN, "[", j + 1, "/", functions, "]", StdOut::GREEN, " Testing: ",
                                  StdOut::BLUE, test_fn.name, StdOut::BLUE,
                                  " ..............", StdOut::GREEN, " Success \n", StdOut::RESET);
                    passed++;
                }
            }
        }
        std::cout << "\n";
        StdOut::Write(StdOut::WHITE, "------------------------- Tests: ", StdOut::GREEN, passed, " succeeded, ", StdOut::RED, failed, " failed ", StdOut::WHITE, "------------------------- ", StdOut::RESET, "\n");
    }
} Unit;

struct AddStuff
{
    AddStuff(const char *SUITE, const char *TEST_NAME, MFunc function)
    {
        Unit.AddTestSuite(SUITE, TEST_NAME, function);
    }
};

#define TEST(SUITE, TEST_NAME) \
void mtest_##SUITE##TEST_NAME(); \
static AddStuff stuff_##SUITE##TEST_NAME(#SUITE, #TEST_NAME, mtest_##SUITE##TEST_NAME); \
void mtest_##SUITE##TEST_NAME() \

#define ASSERT_EXPECT(a, b, error) ((a == b) ? ({}) : (Unit.AddDiagnostic(a, b, __FILE__, "ASSERT_EQ("#a", "#b", "#error");", __LINE__, error, true, true)))
#define ASSERT_EQ(a, b, error) ((a == b) ? ({}) : (Unit.AddDiagnostic(a, b, __FILE__, "ASSERT_EQ("#a", "#b", "#error");", __LINE__, error, true)))
#define ASSERT_NE(a, b, error) ((a != b) ? ({}) : (Unit.AddDiagnostic(a, b, __FILE__, "ASSERT_NE("#a", "#b", "#error");", __LINE__, error, true)))
#define ASSERT_LT(a, b, error) ((a < b) ? ({}) : (Unit.AddDiagnostic(a, b, __FILE__, "ASSERT_LT("#a", "#b", "#error");", __LINE__, error, true)))
#define ASSERT_GT(a, b, error) ((a > b) ? ({}) : (Unit.AddDiagnostic(a, b, __FILE__, "ASSERT_GT("#a", "#b", "#error");", __LINE__, error, true)))
#define ASSERT_LTE(a, b, error) ((a <= b) ? ({}) : (Unit.AddDiagnostic(a, b, __FILE__, "ASSERT_LTE("#a", "#b", "#error");", __LINE__, error, true)))
#define ASSERT_GTE(a, b, error) ((a >= b) ? ({}) : (Unit.AddDiagnostic(a, b, __FILE__, "ASSERT_GTE("#a", "#b", "#error");", __LINE__, error, true)))
#define ASSERT_TRUE(a, error) ((a == true) ? ({}) : (Unit.AddDiagnostic(a, "null", __FILE__, "ASSERT_TRUE("#a", "#error");", __LINE__, error, true)))
#define ASSERT_FALSE(a, error) ((a == false) ? ({}) : (Unit.AddDiagnostic(a, "null", __FILE__, "ASSERT_FALSE("#a", "#error");", __LINE__, error, true)))

#define CHECK_EXPECT(a, b, error) ((a == b) ? ({}) : (Unit.AddDiagnostic(a, b, __FILE__, "CHECK_EXPECT("#a", "#b", "#error");", __LINE__, error, false, true)))
#define CHECK_EQ(a, b, error) ((a == b) ? ({}) : (Unit.AddDiagnostic(a, b, __FILE__, "CHECK_EQ("#a", "#b", "#error");", __LINE__, error, false)))
#define CHECK_NE(a, b, error) ((a != b) ? ({}) : (Unit.AddDiagnostic(a, b, __FILE__, "CHECK_NE("#a", "#b", "#error");", __LINE__, error, false)))
#define CHECK_LT(a, b, error) ((a < b) ? ({}) : (Unit.AddDiagnostic(a, b, __FILE__, "CHECK_LT("#a", "#b", "#error");", __LINE__, error, false)))
#define CHECK_GT(a, b, error) ((a > b) ? ({}) : (Unit.AddDiagnostic(a, b, __FILE__, "CHECK_GT("#a", "#b", "#error");", __LINE__, error, false)))
#define CHECK_LTE(a, b, error) ((a <= b) ? ({}) : (Unit.AddDiagnostic(a, b, __FILE__, "CHECK_LTE("#a", "#b", "#error");", __LINE__, error, false)))
#define CHECK_GTE(a, b, error) ((a >= b) ? ({}) : (Unit.AddDiagnostic(a, b, __FILE__, "CHECK_GTE("#a", "#b", "#error");", __LINE__, error, false)))
#define CHECK_TRUE(a, error) ((a == true) ? ({}) : (Unit.AddDiagnostic(a, "null", __FILE__, "CHECK_TRUE("#a", "#error");", __LINE__, error, false)))
#define CHECK_FALSE(a, error) ((a == false) ? ({}) : (Unit.AddDiagnostic(a, "null", __FILE__, "CHECK_FALSE("#a", "#error");", __LINE__, error, false)))


#   endif //MINT_M_TESTV2_H
