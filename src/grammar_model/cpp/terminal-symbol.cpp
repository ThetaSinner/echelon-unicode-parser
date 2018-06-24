#include "terminal-symbol.hpp"

template<typename T>
SymbolType TerminalSymbol<T>::getType() {
    return SymbolType::Terminal;
}
