#include <gtest/gtest.h>

#include "chomsky-test.hpp"

TEST(ChomskyTest, Test) {
    ASSERT_EQ(1, 1);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
