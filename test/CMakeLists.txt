cmake_minimum_required(VERSION 3.0)

project(test)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)
enable_testing()

file(GLOB TEST_SRCS "*.cc")
add_executable(test ${TEST_SRCS})

target_link_libraries(
  test
  syntacore-test-lib
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test)
