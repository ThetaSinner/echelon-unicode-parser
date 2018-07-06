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
  auto *T = new NonTerminalSymbol<std::string>();
  T->setValue("T");

  auto *b = new TerminalSymbol<std::string>();
  b->setValue("b");

  auto *rule = new ProductionRule<std::string>();
  rule->setKey({T});
  rule->setValue({b});

  Grammar<std::string> grammar;
  grammar.setStartSymbol(T);

  grammar.addNonTerminal(T);
  grammar.addTerminal(b);
  grammar.addProductionRule(rule);

  ASSERT_TRUE(ChomskyTest<std::string>::isTypeThree(&grammar));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
