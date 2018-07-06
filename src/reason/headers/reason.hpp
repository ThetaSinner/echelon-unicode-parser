#ifndef REASON_HPP_INCLUDED
#define REASON_HPP_INCLUDED

#include <stack>
#include <string>
#include <sstream>

namespace echelon { namespace diagnostics {

class Reason {
    std::stack<std::string> reason_stack;
public:
    void addReason(std::string reason);

    void replay(std::stringstream& replay_stream);
};

}}

#endif // REASON_HPP_INCLUDED
