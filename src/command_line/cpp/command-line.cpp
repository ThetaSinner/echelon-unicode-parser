#include <iostream>

#include "grammar-parser.hpp"

#include "antlr4-runtime.h"
#include "EchelonLexer.h"
#include "EchelonParser.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Please provide an input file as argument 1" << std::endl;
        return 0;
    }
    
    std::ifstream stream;
    stream.open(argv[1]);
    antlr4::ANTLRInputStream input(stream);
    EchelonLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    EchelonParser parser(&tokens);

    antlr4::tree::ParseTree *tree = parser.root();
    EchelonCustomListener listener;
    antlr4::tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);

    return 0;
}
