#ifndef SYMBOL_HPP_INCLUDED
#define SYMBOL_HPP_INCLUDED

#include "symbol-type.hpp"

typedef unsigned long SymbolId;

class Symbol {
    SymbolId id = 0;

public:
    SymbolId getId() {
        return id;
    }

    void setId(SymbolId id) {
        this->id = id;
    }

    virtual bool equals(const Symbol *const other) const {
        return other != nullptr && this->id == other->id;
    }

    virtual SymbolType getType() = 0;
};

#endif // SYMBOL_HPP_INCLUDED
