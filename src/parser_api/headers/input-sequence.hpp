#ifndef INPUT_SEQUENCE_HPP_INCLUDED
#define INPUT_SEQUENCE_HPP_INCLUDED

#include <functional>

namespace echelon { namespace parsing { namespace api {

template<typename T>
class InputSequence {
public:
    virtual unsigned length() = 0;
    virtual void each(std::function<void(T&)> action) = 0;
};

}}}

#endif // INPUT_SEQUENCE_HPP_INCLUDED