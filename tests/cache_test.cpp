#include <gtest/gtest.h>

#include "../src/cache2.0.hpp"

TEST(BasicTests, AddingTest) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}
