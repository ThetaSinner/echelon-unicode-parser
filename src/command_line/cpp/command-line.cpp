#include <iostream>

#include "grammar-parser.hpp"

#include "antlr4-runtime.h"
#include "EchelonLexer.h"
#include "EchelonParser.h"

using namespace org::antlr::v4::runtime;

int main(int argc, char** args) {
    if (argc < 2) {
        std::cout << "Please provide an input file as argument 1" << std::endl;
        return 0;
    }
    
    std::ifstream stream;
    stream.open(argv[1]);
    ANTLRInputStream input(stream);
    EchelonLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    EchelonParser parser(&tokens);

    tree::ParseTree *tree = parser.key();
    EchelonCustomListener listener;
    tree::ParseTreeWalker::DEFAULT->walk(&listener, tree);

    return 0;
}
