#ifndef INPUT_SEQUENCE_HPP_INCLUDED
#define INPUT_SEQUENCE_HPP_INCLUDED

#include <array>
#include <experimental/array>

template<typename T>
class InputSequence<T> {
    std::array<T> _input_sequence;
public:
    InputSequence(T[] input_sequence) {
        _input_sequence = std::experimental::to_array(input_sequence);
    }
};

#endif // INPUT_SEQUENCE_HPP_INCLUDED