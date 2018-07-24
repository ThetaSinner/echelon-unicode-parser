#ifndef TERMINAL_SYMBOL_HPP_INCLUDED
#define TERMINAL_SYMBOL_HPP_INCLUDED

#include "symbol.hpp"
#include "symbol-type.hpp"

template<typename Tc>
class TerminalSymbol : public Symbol {
    Tc value;

public:
    SymbolType getType() override {
        return SymbolType::Terminal;
    }

    Tc getValue() {
        return value;
    }

    void setValue(Tc value) {
        this->value = value;
    }
};

#endif // TERMINAL_SYMBOL_HPP_INCLUDED
