cmake_minimum_required(VERSION 3.0)

project(syntacore-test)

include_directories(src)

file(GLOB SRCS "src/*.cc" EXCEPT main.cc)
add_library(syntacore-test-lib ${SRCS})
add_executable(syntacore-test src/main.cc)
target_link_libraries(syntacore-test syntacore-test-lib)

add_subdirectory(test)
