#ifndef SYMBOL_HPP_INCLUDED
#define SYMBOL_HPP_INCLUDED

#include "symbol-type.hpp"

template<typename T>
class Symbol {
    T value;

public:
    virtual T getValue();
    virtual void setValue(T value);

    virtual SymbolType getType() = 0;
};

#include <string>
template class Symbol<std::string>;

#endif // SYMBOL_HPP_INCLUDED