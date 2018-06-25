#ifndef NON_TERMINAL_SYMBOL_HPP_INCLUDED
#define NON_TERMINAL_SYMBOL_HPP_INCLUDED

#include "symbol.hpp"
#include "symbol-type.hpp"

template<typename T>
class NonTerminalSymbol : public Symbol<T>  {
public:
    SymbolType getType() override {
        return SymbolType::NonTerminal;
    }
};

#endif // NON_TERMINAL_SYMBOL_HPP_INCLUDED
