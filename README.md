# MUnit Unit Testing Software
A short, simple, and easy to use single header file libary for unit-testing software which has no dependencies other that the STL. In less than 400 lines of code, it provides easy to use testing suites without any hastle.

## Static Library Configuration
```
cmake_minimum_required(VERSION 3.13.4)
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
