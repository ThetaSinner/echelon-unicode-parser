#include <benchmark/benchmark.h>

#include "input-sequence.hpp"
#include "string-input-sequence.hpp"
#include "sequence-partition.hpp"
#include "sequence-partition-generator.hpp"
#include "unger-parser.hpp"
#include "terminal-symbol.hpp"
#include "non-terminal-symbol.hpp"

using namespace echelon::parsing;

auto makeUngerParserWithSimpleArithmeticGrammar() {

  auto symbol_Expr = std::make_shared<NonTerminalSymbol<std::string>>();
  symbol_Expr->setValue("E");
  symbol_Expr->setId(1);

  auto symbol_Term = std::make_shared<NonTerminalSymbol<std::string>>();
  symbol_Term->setValue("T");
  symbol_Term->setId(2);

  auto symbol_Factor = std::make_shared<NonTerminalSymbol<std::string>>();
  symbol_Factor->setValue("F");
  symbol_Factor->setId(3);

  auto symbol_plus = std::make_shared<TerminalSymbol<char>>();
  symbol_plus->setValue('+');
  symbol_plus->setId(400);

  auto symbol_times = std::make_shared<TerminalSymbol<char>>();
  symbol_times->setValue('x');
  symbol_times->setId(401);

  auto symbol_open_paren = std::make_shared<TerminalSymbol<char>>();
  symbol_open_paren->setValue('(');
  symbol_open_paren->setId(403);

  auto symbol_close_paren = std::make_shared<TerminalSymbol<char>>();
  symbol_close_paren->setValue(')');
  symbol_close_paren->setId(404);

  auto symbol_i = std::make_shared<TerminalSymbol<char>>();
  symbol_i->setValue('i');
  symbol_i->setId(500);

  auto rule_expr_recurse = std::make_shared<ProductionRule>();
  rule_expr_recurse->setKey({symbol_Expr});
  rule_expr_recurse->setValue({symbol_Expr, symbol_plus, symbol_Term});

  auto rule_expr_to_term = std::make_shared<ProductionRule>();
  rule_expr_to_term->setKey({symbol_Expr});
  rule_expr_to_term->setValue({symbol_Term});

  auto rule_term_recurse = std::make_shared<ProductionRule>();
  rule_term_recurse->setKey({symbol_Term});
  rule_term_recurse->setValue({symbol_Term, symbol_times, symbol_Factor});

  auto rule_term_to_factor = std::make_shared<ProductionRule>();
  rule_term_to_factor->setKey({symbol_Term});
  rule_term_to_factor->setValue({symbol_Factor});

  auto rule_factor_nest_expr = std::make_shared<ProductionRule>();
  rule_factor_nest_expr->setKey({symbol_Factor});
  rule_factor_nest_expr->setValue({symbol_open_paren, symbol_Expr, symbol_close_paren});

  auto rule_factor_to_i = std::make_shared<ProductionRule>();
  rule_factor_to_i->setKey({symbol_Factor});
  rule_factor_to_i->setValue({symbol_i});

  using namespace echelon::grammar_model;
  auto *simple_arithmetic_grammar = new Grammar();
  simple_arithmetic_grammar->setStartSymbol(symbol_Expr);

  simple_arithmetic_grammar->addNonTerminal(symbol_Expr);
  simple_arithmetic_grammar->addNonTerminal(symbol_Term);
  simple_arithmetic_grammar->addNonTerminal(symbol_Factor);

  simple_arithmetic_grammar->addTerminal(symbol_plus);
  simple_arithmetic_grammar->addTerminal(symbol_times);
  simple_arithmetic_grammar->addTerminal(symbol_open_paren);
  simple_arithmetic_grammar->addTerminal(symbol_close_paren);
  simple_arithmetic_grammar->addTerminal(symbol_i);

  simple_arithmetic_grammar->addProductionRule(rule_expr_recurse);
  simple_arithmetic_grammar->addProductionRule(rule_expr_to_term);
  simple_arithmetic_grammar->addProductionRule(rule_term_recurse);
  simple_arithmetic_grammar->addProductionRule(rule_term_to_factor);
  simple_arithmetic_grammar->addProductionRule(rule_factor_nest_expr);
  simple_arithmetic_grammar->addProductionRule(rule_factor_to_i);

  using namespace echelon::parsing::impl::unger;
  auto parser = new UngerParser<char>();
  parser->setGrammar(simple_arithmetic_grammar);

  return parser;
}

auto ungerParserWithSimpleArithmeticGrammar = makeUngerParserWithSimpleArithmeticGrammar();

/**
 * Test the Unger parser with the simple arithemtic grammar against single symbol expression
 */
static void BM_ParseSingleSymbolExpression(benchmark::State& state) {
  auto single_symbol_expression = new api_default_impl::StringInputSequence("i");
  for (auto _ : state)
    ungerParserWithSimpleArithmeticGrammar->parse(single_symbol_expression);
}
BENCHMARK(BM_ParseSingleSymbolExpression);

/**
 * Test the Unger parser with the simple arithemtic grammar against a short expression
 */
static void BM_ParseShortExpression(benchmark::State& state) {
  auto short_expression = new api_default_impl::StringInputSequence("(i+i)xi");
  for (auto _ : state)
    ungerParserWithSimpleArithmeticGrammar->parse(short_expression);
}
// Register the function as a benchmark
BENCHMARK(BM_ParseShortExpression);

// Define another benchmark
static void BM_StringCopy(benchmark::State& state) {
  std::string x = "hello";
  for (auto _ : state)
    std::string copy(x);
}
BENCHMARK(BM_StringCopy);
