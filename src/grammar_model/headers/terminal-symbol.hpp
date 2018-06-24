#ifndef TERMINAL_SYMBOL_HPP_INCLUDED
#define TERMINAL_SYMBOL_HPP_INCLUDED

#include "symbol.hpp"
#include "symbol-type.hpp"

template<typename T>
class TerminalSymbol : public Symbol<T>  {
    SymbolType getType() override;
};

#include<string>
template class TerminalSymbol<std::string>;

#endif // TERMINAL_SYMBOL_HPP_INCLUDED