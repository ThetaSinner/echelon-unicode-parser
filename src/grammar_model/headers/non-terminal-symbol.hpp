#ifndef NON_TERMINAL_SYMBOL_HPP_INCLUDED
#define NON_TERMINAL_SYMBOL_HPP_INCLUDED

#include "symbol.hpp"
#include "symbol-type.hpp"

template<typename Tc>
class NonTerminalSymbol : public Symbol  {
    Tc value;

public:
    SymbolType getType() override {
        return SymbolType::NonTerminal;
    }

    Tc getValue() const {
        return value;
    }

    void setValue(Tc value) {
        this->value = value;
    }
};

#endif // NON_TERMINAL_SYMBOL_HPP_INCLUDED
