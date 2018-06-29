#ifndef REASON_HPP_INCLUDED
#define REASON_HPP_INCLUDED

#include <stack>
#include <string>
#include <iostream>

namespace echelon {

class Reason {
    std::stack<std::string> reason_stack;
public:
    void addReason(std::string reason);

    void replay();
};

}

#endif // REASON_HPP_INCLUDED
