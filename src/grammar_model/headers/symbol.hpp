#ifndef SYMBOL_HPP_INCLUDED
#define SYMBOL_HPP_INCLUDED

#include <memory>

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

    virtual bool equals(const std::shared_ptr<const Symbol> other) const {
        return other != nullptr && this->id == other->id;
    }

    virtual SymbolType getType() = 0;
};

#endif // SYMBOL_HPP_INCLUDED
