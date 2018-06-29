#include "reason.hpp"

void echelon::Reason::addReason(std::string reason) {
    reason_stack.push(reason);
}

void echelon::Reason::replay() {
    if (reason_stack.size()) {
        std::cout << "Reason: " << reason_stack.top() << "\n";
        reason_stack.pop();
    }
    else {
        std::cout << "Unknown reason\n";
        return;
    }

    while (reason_stack.size()) {
        std::cout << "        " << reason_stack.top() << "\n";
        reason_stack.pop();
    }

    std::cout << std::flush;
}
