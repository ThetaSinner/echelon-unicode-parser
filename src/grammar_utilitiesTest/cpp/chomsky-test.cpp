#include <gtest/gtest.h>

#include <string>

#include "grammar.hpp"
#include "non-terminal-symbol.hpp"
#include "terminal-symbol.hpp"
#include "production-rule.hpp"

#include "chomsky-test.hpp"

using namespace echelon::grammar_model;
using namespace echelon::grammar_utilities;

TEST(ChomskyTest, ValidTypeThreeGrammar) {
  auto T = std::make_shared<NonTerminalSymbol<std::string>>();
  T->setValue("T");

  auto b = std::make_shared<TerminalSymbol<std::string>>();
  b->setValue("b");

  auto rule = std::make_shared<ProductionRule>();
  rule->setKey({T});
  rule->setValue({b});

  Grammar grammar;
  grammar.setStartSymbol(T);

  grammar.addNonTerminal(T);
  grammar.addTerminal(b);
  grammar.addProductionRule(rule);

  ASSERT_TRUE(ChomskyTest::isTypeThree(&grammar));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
