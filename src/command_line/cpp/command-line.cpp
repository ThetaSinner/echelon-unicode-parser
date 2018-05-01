#include <iostream>

#include "grammar-parser.hpp"

int main(int argc, char** args) {
    std::cout << "You passed " << argc << "args." << std::endl;
    GrammarParser *g = new GrammarParser();
    g->test();
    return 0;
}
