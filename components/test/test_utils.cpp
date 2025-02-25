//// filepath: test/test_utils.cpp
#include <gtest/gtest.h>
#include "utils.h"

// A simple test for the uint8_starts_with_str helper.
TEST(S21Test, Uint8StrcompMatches) {
  std::vector<uint8_t> vec = {'F', '1', 'X'};
  const char* str = "F1";
  EXPECT_TRUE(uint8_starts_with_str(vec, str));

  vec = {'F', '1', 'X', 'Y'};
  str = "F1XY";
  EXPECT_TRUE(uint8_starts_with_str(vec, str));

  vec = {'F', '1', 'X', 'Y'};
  str = "G1XY";
  EXPECT_FALSE(uint8_starts_with_str(vec, str));

  vec = {'F', 'Y', '0', '0'};
  str = "FY0";
  EXPECT_TRUE(uint8_starts_with_str(vec, str));

  vec = {'G', '8', '0', '0'};
  str = "G8";
  EXPECT_TRUE(uint8_starts_with_str(vec, str));
}
