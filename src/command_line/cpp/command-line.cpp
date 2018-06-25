#include <iostream>
#include <string>
#include <iostream>

#include "grammar-parser.hpp"
#include "terminal-symbol.hpp"
#include "non-terminal-symbol.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Please provide an input file as argument 1" << std::endl;
        return 0;
    }
    
    // std::ifstream stream;
    // stream.open(argv[1]);
    
    GrammarParser grammarParser;
    grammarParser.test();

    Symbol<std::string> *symbolT = new TerminalSymbol<std::string>();
    
    if (symbolT->getType() == SymbolType::NonTerminal) {
        std::cout << "symbolT is a non-terminal\n";
    }
    else {
        std::cout << "symbolT is a terminal\n";
    }

    delete symbolT;

    symbolT = new NonTerminalSymbol<std::string>();

    if (symbolT->getType() == SymbolType::NonTerminal) {
        std::cout << "Now, symbolT is a non-terminal\n";
    }
    else {
        std::cout << "Now, symbolT is a terminal\n";
    }

    std::cout << "bye" << std::endl;
    return 0;
}
