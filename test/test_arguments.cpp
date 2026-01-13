#include <gtest/gtest.h>

extern "C" {
#include "arguments.h"
}

TEST(Arguments, NoArg) {
  args_t args;
  EXPECT_FALSE(parse_args(0, (const char*[]){}, &args));
}

TEST(Arguments, InvalidArg) {
  args_t args;
  EXPECT_FALSE(parse_args(0, (const char*[]){"nope"}, &args));
}
