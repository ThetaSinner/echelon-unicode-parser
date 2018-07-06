#include "reason.hpp"

using namespace echelon::diagnostics;

void Reason::addReason(std::string reason) {
    reason_stack.push(reason);
}

void Reason::replay(std::stringstream& replay_stream) {
    if (reason_stack.size()) {
        replay_stream << "Reason: " << reason_stack.top();
        reason_stack.pop();
    }
    else {
        replay_stream << "Unknown reason";
        return;
    }

    while (reason_stack.size()) {
        replay_stream << "\n        " << reason_stack.top();
        reason_stack.pop();
    }
}
