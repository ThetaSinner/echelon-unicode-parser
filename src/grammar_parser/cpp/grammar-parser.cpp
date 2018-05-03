#include "grammar-parser.hpp"

#include <iostream>

void EchelonCustomListener::enterNumber(EchelonParser::NumberContext *ctx) {
    std::cout << "entering number" << std::endl;
}
