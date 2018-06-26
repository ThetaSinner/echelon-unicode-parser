#ifndef RULE_HPP_INCLUDED
#define RULE_HPP_INCLUDED

#include <list>
#include <iostream>

#include "symbol.hpp"
#include "symbol-type.hpp"

template<typename T>
class ProductionRule {
    std::list<Symbol<T>*> key;
    std::list<Symbol<T>*> value;

public:
    void setKey(std::list<Symbol<T>*> key) {
        this->key = key;
    }

    void setValue(std::list<Symbol<T>*> value) {
        this->value = value;
    }

    bool isValid() {
        bool leftHandSideHasTerminal = false;
        for (auto k : key) {
            if (k->getType() == SymbolType::Terminal) {
                leftHandSideHasTerminal = true;
                break;
            }
        }

        return leftHandSideHasTerminal;
    }

    void print() {
        for (auto k : key) {
            std::cout << k->getValue();
        }

        std::cout << " -> ";

        for (auto v : value) {
            std::cout << v->getValue();
        }

        std::cout << "\n";

        if (this->isValid()) {
            std::cout << "The rule is valid";
        }
        else {
            std::cout << "The rule is NOT valid";
        }

        std::cout << std::endl;
    }
};

#endif // RULE_HPP_INCLUDED
