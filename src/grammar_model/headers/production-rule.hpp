#ifndef RULE_HPP_INCLUDED
#define RULE_HPP_INCLUDED

#include <list>
#include <iostream>
#include <functional>
#include <algorithm>

#include "symbol.hpp"
#include "symbol-type.hpp"

class ProductionRule {
protected:
    std::list<Symbol*> key;
    std::list<Symbol*> value;

public:
    void setKey(std::list<Symbol*> key) {
        this->key = key;
    }

    void setValue(std::list<Symbol*> value) {
        this->value = value;
    }

    unsigned keyLength() {
        return key.size();
    }

    unsigned valueLength() {
        return value.size();
    }

    Symbol* getFirstKeySymbol() {
        return key.front();
    }

    Symbol* getLastValueSymbol() {
        return value.back();
    }

    void eachValueSymbol(std::function<void(Symbol*)> action) const {
        std::for_each(value.begin(), value.end(), action);
    }

    bool isValid() {
        bool leftHandSideHasNonTerminal = false;
        for (auto k : key) {
            if (k->getType() == SymbolType::NonTerminal) {
                leftHandSideHasNonTerminal = true;
                break;
            }
        }

        return leftHandSideHasNonTerminal;
    }
};

#endif // RULE_HPP_INCLUDED
