#ifndef RULE_HPP_INCLUDED
#define RULE_HPP_INCLUDED

#include <list>
#include <iostream>
#include <functional>
#include <algorithm>

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

    unsigned keyLength() {
        return key.size();
    }

    unsigned valueLength() {
        return value.size();
    }

    Symbol<T>* getFirstKeySymbol() {
        return key.front();
    }

    Symbol<T>* getLastValueSymbol() {
        return value.back();
    }

    void eachValueSymbol(std::function<void(Symbol<T>*)> action) const {
        std::for_each(value.begin(), value.end(), action);
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
