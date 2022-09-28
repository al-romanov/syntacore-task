#include <gtest/gtest.h>
#include <query_tree.h>

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <stdexcept>
#include <vector>

TEST(NthSmallest, SimpleTest) {
  std::vector<int64_t> values = { 3,   2,  1,   -1, -2, -4,   123,
                                  -43, 23, 435, 45, 76, -2345 };
  syntacore::QueryTree qt;
  for (auto v : values) { qt.Insert(v); }
  std::sort(values.begin(), values.end());
  for (size_t i = 0; i < values.size(); ++i) {
    EXPECT_EQ(qt.NthSmallest(i + 1), values[i]);
  }
}

TEST(NthSmallest, RandomNumbersTest) {
  std::vector<int64_t> values{ 1000 };
  syntacore::QueryTree qt;
  for (auto &v : values) {
    v = rand();
    qt.Insert(v);
  }
  std::sort(values.begin(), values.end());
  for (size_t i = 0; i < values.size(); ++i) {
    EXPECT_EQ(qt.NthSmallest(i + 1), values[i]);
  }
}

TEST(NthSmallest, UncorrectArgumentTest) {
  bool exception_caught = false;
  syntacore::QueryTree qt;
  qt.Insert(34);
  try {
    qt.NthSmallest(10001);
  } catch (std::invalid_argument &exc) { exception_caught = true; }
  EXPECT_EQ(exception_caught, true);
  exception_caught = false;
  try {
    qt.NthSmallest(0);
  } catch (std::invalid_argument &exc) { exception_caught = true; }
  EXPECT_EQ(exception_caught, true);
}
