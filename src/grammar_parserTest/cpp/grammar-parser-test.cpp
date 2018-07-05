#include <gtest/gtest.h>

#include "grammar-parser.hpp"

TEST(GrammarParserTest, Test) {
    GrammarParser p;
    p.test();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
