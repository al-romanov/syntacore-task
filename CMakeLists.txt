cmake_minimum_required(VERSION 3.0)

project(syntacore-task)

include_directories(include)

file(GLOB SRCS "src/*.cc" EXCEPT src/main.cc)
list(FILTER SRCS EXCLUDE REGEX ".*main\\.cc$")
add_library(syntacore_task_lib ${SRCS})
add_executable(syntacore_task src/main.cc)
target_link_libraries(syntacore_task syntacore_task_lib)

add_subdirectory(test)
