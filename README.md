MUnit : short, simple, and easy to use testing software
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



## Static Library Configuration
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

## Testing Project Subdirectory Configuration
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
