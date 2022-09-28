cmake_minimum_required(VERSION 3.0)

project(syntacore-test)

include_directories(include)

file(GLOB SRCS "src/*.cc" EXCEPT src/main.cc)
list(FILTER SRCS EXCLUDE REGEX ".*main\\.cc$")
add_library(syntacore_test_lib ${SRCS})
add_executable(syntacore_test src/main.cc)
target_link_libraries(syntacore_test syntacore_test_lib)

add_subdirectory(test)
