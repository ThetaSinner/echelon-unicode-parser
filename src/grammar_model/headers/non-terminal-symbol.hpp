#ifndef NON_TERMINAL_SYMBOL_HPP_INCLUDED
#define NON_TERMINAL_SYMBOL_HPP_INCLUDED

#include "symbol.hpp"
#include "symbol-type.hpp"

template<typename T>
class NonTerminalSymbol : public Symbol  {
    T value;

public:
    SymbolType getType() override {
        return SymbolType::NonTerminal;
    }

    T getValue() const {
        return value;
    }

    void setValue(T value) {
        this->value = value;
    }
};

#endif // NON_TERMINAL_SYMBOL_HPP_INCLUDED
