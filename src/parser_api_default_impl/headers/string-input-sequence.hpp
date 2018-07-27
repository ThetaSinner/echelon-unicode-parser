#ifndef STRING_INPUT_SEQUENCE_HPP_INCLUDED
#define STRING_INPUT_SEQUENCE_HPP_INCLUDED

#include <string>
#include "input-sequence.hpp"

namespace echelon { namespace parsing {namespace api_default_impl {

class StringInputSequence : public echelon::parsing::api::InputSequence<char> {
    std::string _input_sequence;
public:
    StringInputSequence(std::string input_sequence) {
        _input_sequence = input_sequence;
    }

    unsigned length() override {
        return _input_sequence.size();
    }

    void each(std::function<void(char&)> action) override {
        std::for_each(_input_sequence.begin(), _input_sequence.end(), action);
    }

    StringInputSequence* getSubSequence(unsigned start, unsigned length) override {
        if (length == 0) {
            return nullptr;
        }

        return new StringInputSequence(_input_sequence.substr(start, length));
    }

    bool matches(char element) {
        return _input_sequence.length() == 1 && _input_sequence[0] == element;
    }
};
    
}}}

#endif // STRING_INPUT_SEQUENCE_HPP_INCLUDED
