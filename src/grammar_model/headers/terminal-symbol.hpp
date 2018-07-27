#ifndef TERMINAL_SYMBOL_HPP_INCLUDED
#define TERMINAL_SYMBOL_HPP_INCLUDED

#include "symbol.hpp"
#include "symbol-type.hpp"

template<typename T>
class TerminalSymbol : public Symbol {
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
};

#endif // TERMINAL_SYMBOL_HPP_INCLUDED
