#ifndef RULE_HPP_INCLUDED
#define RULE_HPP_INCLUDED

#include <list>
#include <iostream>
#include <functional>
#include <algorithm>
#include <memory>

#include "symbol.hpp"
#include "symbol-type.hpp"

class ProductionRule {
protected:
    std::vector<std::shared_ptr<Symbol>> key;
    std::vector<std::shared_ptr<Symbol>> value;

public:
    void setKey(std::vector<std::shared_ptr<Symbol>> key) {
        this->key = key;
    }

    void setValue(std::vector<std::shared_ptr<Symbol>> value) {
        this->value = value;
    }

    unsigned keyLength() {
        return key.size();
    }

    unsigned valueLength() {
        return value.size();
    }

    std::shared_ptr<Symbol> getFirstKeySymbol() {
        return key.front();
    }

    std::shared_ptr<Symbol> getLastValueSymbol() {
        return value.back();
    }

    const std::vector<std::shared_ptr<Symbol>>& getValues() const {
        return value;
    }

    void eachValueSymbol(std::function<void(std::shared_ptr<Symbol>)> action) const {
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
