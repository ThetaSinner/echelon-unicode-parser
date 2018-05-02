#include "grammar-parser.hpp"

#include <iostream>

EchelonCustomListener::enterNumber(ParserRuleContext *ctx) {
    std::cout << "entering number" << std::endl;
}
