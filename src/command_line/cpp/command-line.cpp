#include <iostream>
#include <string>
#include <iostream>

#include "grammar-parser.hpp"
#include "terminal-symbol.hpp"
#include "non-terminal-symbol.hpp"
#include "rule.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Please provide an input file as argument 1" << std::endl;
        return 0;
    }
    
    // std::ifstream stream;
    // stream.open(argv[1]);
    
    GrammarParser grammarParser;
    grammarParser.test();

    auto *symbol_T = new TerminalSymbol<std::string>();
    symbol_T->setValue("T");
    
    if (symbol_T->getType() == SymbolType::NonTerminal) {
        std::cout << "symbol_T is a non-terminal\n";
    }
    else {
        std::cout << "symbol_T is a terminal\n";
    }

    auto *symbol_b = new NonTerminalSymbol<std::string>();
    symbol_b->setValue("b");

    if (symbol_b->getType() == SymbolType::NonTerminal) {
        std::cout << "symbol_b is a non-terminal\n";
    }
    else {
        std::cout << "symbol_b is a terminal\n";
    }

    auto r = new Rule<std::string>();
    r->setKey({symbol_T});
    r->setValue({symbol_b});

    r->print();

    std::cout << "bye" << std::endl;
    return 0;
}
