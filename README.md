# MUnit Unit Testing Software
A short, simple, and easy to use single header file libary for unit-testing software which has no dependencies other that the STL. In less than 400 lines of code, it provides easy to use testing suites without any hastle.

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
