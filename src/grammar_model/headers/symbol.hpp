#ifndef SYMBOL_HPP_INCLUDED
#define SYMBOL_HPP_INCLUDED

#include "symbol-type.hpp"

template<typename T>
class Symbol {
    T value;

public:
    virtual T getValue() {
        return this->value;
    }

    virtual void setValue(T value) {
        this->value = value;
    }

    virtual SymbolType getType() = 0;
};

#endif // SYMBOL_HPP_INCLUDED
