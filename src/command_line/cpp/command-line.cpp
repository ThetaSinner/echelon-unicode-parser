#include <iostream>
#include <string>
#include <iostream>
#include <vector>

#include "grammar-parser.hpp"
#include "terminal-symbol.hpp"
#include "non-terminal-symbol.hpp"
#include "production-rule.hpp"
#include "grammar.hpp"

#include "chomsky-test.hpp"

#include "reason.hpp"

#include "cleaner.hpp"

#include "input-sequence.hpp"
#include "string-input-sequence.hpp"
#include "sequence-partition.hpp"
#include "sequence-partition-generator.hpp"
#include "unger-parser.hpp"

void printIndices(std::vector<unsigned> indices) {
    for (unsigned i = 0; i < indices.size(); i++) {
        std::cout << indices[i];
        if (i + 1 != indices.size()) {
            std::cout << " | ";
        }
    }

    std::cout << "\n";
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Please provide an input file as argument 1" << std::endl;
        return 0;
    }
    
    // std::ifstream stream;
    // stream.open(argv[1]);
    
    GrammarParser grammarParser;
    grammarParser.test();

    auto *symbol_T = new NonTerminalSymbol<std::string>();
    symbol_T->setValue("T");
    
    if (symbol_T->getType() == SymbolType::NonTerminal) {
        std::cout << "symbol_T is a non-terminal\n";
    }
    else {
        std::cout << "symbol_T is a terminal\n";
    }

    auto *symbol_b = new TerminalSymbol<std::string>();
    symbol_b->setValue("b");

    if (symbol_b->getType() == SymbolType::NonTerminal) {
        std::cout << "symbol_b is a non-terminal\n";
    }
    else {
        std::cout << "symbol_b is a terminal\n";
    }

    auto r = new ProductionRule();
    r->setKey({symbol_T});
    r->setValue({symbol_b});

    auto badRule = new ProductionRule();
    badRule->setKey({symbol_b});
    badRule->setValue({symbol_T});

    using namespace echelon::parsing;
    api::InputSequence<char>* seq = new api_default_impl::StringInputSequence("pqr");

    auto* gen = new impl::unger::internal::SequencePartitionGenerator<char>(seq, 3);
    while (gen->currentValue() != nullptr) {
        auto indices = gen->currentValue()->getPartitionIndices();
        printIndices(indices);

        gen->moveNext();
    }

    auto *symbol_Expr = new NonTerminalSymbol<char>();
    symbol_Expr->setValue('E');

    auto *symbol_Term = new NonTerminalSymbol<std::string>();
    symbol_Term->setValue("T");

    auto *symbol_Factor = new NonTerminalSymbol<std::string>();
    symbol_Factor->setValue("F");

    auto *symbol_plus = new TerminalSymbol<std::string>();
    symbol_plus->setValue("+");

    auto *symbol_times = new TerminalSymbol<std::string>();
    symbol_times->setValue("x");

    auto *symbol_open_paren = new TerminalSymbol<std::string>();
    symbol_open_paren->setValue("(");

    auto *symbol_close_paren = new TerminalSymbol<std::string>();
    symbol_close_paren->setValue(")");

    auto *symbol_i = new TerminalSymbol<std::string>();
    symbol_i->setValue("i");

    auto *rule_expr_recurse = new ProductionRule();
    rule_expr_recurse->setKey({symbol_Expr});
    rule_expr_recurse->setValue({symbol_Expr, symbol_plus, symbol_Term});

    auto *rule_expr_to_term = new ProductionRule();
    rule_expr_to_term->setKey({symbol_Expr});
    rule_expr_to_term->setValue({symbol_Term});

    auto *rule_term_recurse = new ProductionRule();
    rule_term_recurse->setKey({symbol_Term});
    rule_term_recurse->setValue({symbol_Term, symbol_times, symbol_Factor});

    auto *rule_term_to_factor = new ProductionRule();
    rule_term_to_factor->setKey({symbol_Term});
    rule_term_to_factor->setValue({symbol_Factor});

    auto *rule_factor_nest_expr = new ProductionRule();
    rule_factor_nest_expr->setKey({symbol_Factor});
    rule_factor_nest_expr->setValue({symbol_open_paren, symbol_Expr, symbol_close_paren});

    auto *rule_factor_to_i = new ProductionRule();
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

    // Test input sequence
    api::InputSequence<char>* seq2 = new api_default_impl::StringInputSequence("(i+i)xi");

    using namespace echelon::parsing::impl::unger;
    UngerParser<char> parser;
    parser.parse(simple_arithmetic_grammar, seq2);

    std::cout << "bye" << std::endl;
    return 0;
}
