#include <iostream>
#include <string>
#include <iostream>

#include "grammar-parser.hpp"
#include "terminal-symbol.hpp"
#include "non-terminal-symbol.hpp"
#include "production-rule.hpp"
#include "grammar.hpp"

#include "chomsky_test.hpp"

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

    auto r = new ProductionRule<std::string>();
    r->setKey({symbol_T});
    r->setValue({symbol_b});

    r->print();

    auto badRule = new ProductionRule<std::string>();
    badRule->setKey({symbol_b});
    badRule->setValue({symbol_T});

    badRule->print();

    std::cout << "\nYour grammar is: \n";
    Grammar<std::string> g;
    g.addNonTerminal(symbol_T);
    g.setStartSymbol(symbol_T);
    g.addTerminal(symbol_b);
    g.addProductionRule(r);

    g.print();

    using namespace echelon::grammar_utilities;
    if (ChomskyTest<std::string>::isTypeThree(&g)) {
        std::cout << "g is a type 3 grammar\n";
    }
    else {
        std::cout << "g is NOT a type 3 grammar\n";
    }

    std::cout << "bye" << std::endl;
    return 0;
}
