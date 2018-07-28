#ifndef TERMINAL_SYMBOL_HPP_INCLUDED
#define TERMINAL_SYMBOL_HPP_INCLUDED

#include "symbol.hpp"
#include "symbol-type.hpp"

template<typename T>
class TerminalSymbol : public Symbol {
    bool empty = false;
    T value;

public:
    SymbolType getType() override {
        return SymbolType::Terminal;
    }

    T getValue() {
        return value;
    }

    void setValue(T value) {
        this->value = value;
    }

    bool isEmpty() {
        return empty;
    }

    void setEmpty(bool empty) {
        this->empty = empty;
    }
};

#endif // TERMINAL_SYMBOL_HPP_INCLUDED
