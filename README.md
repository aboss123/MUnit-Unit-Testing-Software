MUnit : Short, Simple, and easy to use unit-testing software
===============================================
Unit Testing is used everywhere now, whether it be in production applications at Google, or at just a side project.
Testing is very important to do, and un-tested code could lead to potential security vulnerabilites. This is why
I developed MTest. MTest is short (less than 400 lines) with the ability to easily group a wide range of tests 
under a test suite, not only that, but it has allowed me to easily integrate testing without any overhead of linking at
all.

Table of Contents
-----------------

* [Get Started](#get-started)
* [Documentation](#documentation)
* [Static-Library-Configuration](#static-library-configuration)
* [Subdirectory-Configuration](#subdirectory-configuration)

Get Started
-----------


The MUnit library is easily consumable with a single .h and .cpp file.

0. Prerequisites: `g++` (version 7 or better) or `clang++` (version 6 or better), or in general a [C++17] compliant compiler.

1. Clone the repository on GitHub
   ```
   git clone https://github.com/aboss123/MUnit-Unit-Testing-Software.git
   ```
2. Create yout `main.cpp`:

   ```c++
   #include "m_testv2.h"
   int main(void) {
     Unit.RunTests();
   }
   ```
3. `c++ -o mytests m_testv2.h main.cpp`
4. `./mytests`
   ```
   ------------------------- Tests: 0 succeeded, 0 failed ------------------------- 
   ```
5. Enjoy using the library!

Documentation
-------------
If you have used previous unit-testing software before, you may understand some of the macros in the library.
Some of which include: ```ASSERT_EXPECT``` and ```CHECK_EQ```. These check the different values in your assertion.
To get a clear picture of what they do, you can look at the bottom 30 lines of the ``m_testv2.h`` file and see
what comparison is being done for each assertion or check.

## Example Code Assertion Failure
```c++
#include "m_testv2.h"

TEST(MyTestSuite, MyTest) {
    int a = 2;
    for (int i = 0; i < 23; ++i) {
        a += i * 3;
    }
    ASSERT_EXPECT(a, 759, "NOO");
}

int main(void) {
   Unit.RunTests();
}
```
After compiling and running the output you get is:
```
-------------------------
 Running Suite: MyTestSuite
-------------------------
[1/1] Testing: MyTest .............. Failure 
/home/aboss/Documents/testtest/main.cpp:104:4: error: NOO
 Expected : 759
 Got      : 761

  103   |
> 104   |      ASSERT_EXPECT(a, 759, "NOO");
  105   |~~~~~~^

------------------------- Tests: 0 succeeded, 1 failed ------------------------- 
```
This is what happens when there is an assertion failure. Assertion failures **stop execution** of
all other tests in a particular suite will check failures print out the error messages but still
execute the other tests. 

## Running Multiple Tests and Suites
```c++
#include "m_testv2.h"

TEST(MyTestSuite, NiceTest) {
   // do nothing i guess?
   int x = 23;
   CHECK_EQ(x, 33, "WHAT");
}

TEST(MyTestSuite, MyTest) {
    int a = 0;
    for (int i = 0; i < 23; ++i) {
        a += i * 3;
    }
    ASSERT_EXPECT(a, 759, "NOO");
}

TEST(AnotherSuite, AnotherTest) {
   int x = 23;
   CHECK_EQ(x, 23, "How?");
}

TEST(AnotherSuite, CoolTest) {
   const char *text = "WowCool";
   ASSERT_EXPECT(sizeof("WowCool"), 6, "NOO");
}

int main(void) {
   Unit.RunTests();
}
```
The output for this is:
```
-------------------------
 Running Suite: MyTestSuite
-------------------------
[1/2] Testing: NiceTest .............. Failure 
/home/aboss/Documents/testtest/main.cpp:6:4: error: WHAT
 V1: 23
 V2: 33

  5   |
> 6   |      CHECK_EQ(x, 33, "WHAT");
  7   |~~~~^
[2/2] Testing: MyTest .............. Success 
-------------------------
 Running Suite: AnotherSuite
-------------------------
[1/2] Testing: AnotherTest .............. Success 
[2/2] Testing: CoolTest .............. Failure 
/home/aboss/Documents/testtest/main.cpp:24:4: error: NOO
 Expected : 6
 Got      : 8

  23   |
> 24   |      ASSERT_EXPECT(sizeof("WowCool"), 6, "NOO");
  25   |~~~~~^

------------------------- Tests: 2 succeeded, 2 failed ------------------------- 
```
Notice how the ```CHECK_EQ``` fails but the next test continues to execute. The library
is as simple as that. 

Static Library Configuration
-------------
```
cmake_minimum_required(VERSION YOUR_CMAKE_VERSION)
project(MTest)

set (CURRENT_DIR .)
get_filename_component(PARENT_DIR ${CURRENT_DIR} PATH)

include_directories(${CURRENT_DIR}/include)

add_library(MTest STATIC
        include/m_testv2.h 
        )

# Set Linker Language To C++
set_target_properties(MTest PROPERTIES LINKER_LANGUAGE CXX)
```
To set this up with CMake, make sure you have this file in a folder with the confguration similar
to above, of course, this could be compiled as a dynamic library too, but here is a static one for
reference.

Subdirectory Configuration
```
cmake_minimum_required(VERSION YOUR_CMAKE_VERSION)
project(test)

...

# Setup current and parent directories
set (CURRENT_DIR ${CMAKE_SOURCE_DIR})
get_filename_component(PARENT_DIR ${CURRENT_DIR} PATH)

include_directories(${CURRENT_DIR}/PATH_TO_MTEST)

# Link against MTest
target_link_libraries(test MTest)
```
This is how you may setup adding this to your tests folder of your project.

## General Use
For a single header file library it sometimes makes sense to just include rather than linking it dynamically or statcically.
In that case, you can jus include the file in the project directly.

## License
This code is licensed under the terms of the MIT License.
