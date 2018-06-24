#include "symbol.hpp"

template<typename T>
T Symbol<T>::getValue() {
    return this->value;
}

template<typename T>
void Symbol<T>::setValue(T value) {
    this->value = value;
}
