#include <gtest/gtest.h>

#include <string>

#include "terminal-symbol.hpp"
#include "symbol-type.hpp"

TEST(TerminalSymbolTest, CheckSymbolType) {
    TerminalSymbol<std::string> testSymbol;

    auto actualType = testSymbol.getType();
    auto expectedType = SymbolType::Terminal;
    EXPECT_EQ(actualType, expectedType);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
