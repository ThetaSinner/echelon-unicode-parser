#ifndef CLEANABLE_HPP_INCLUDED
#define CLEANABLE_HPP_INCLUDED

#include "clean-status.hpp"

namespace echelon { namespace grammar_utilities { namespace cleaning {

template<typename T>
class Cleanable {
    CleanStatus clean_status;
protected:
    CleanableTerminalSymbol() {
        clean_status = CleanStatus::DoNotKnow
    }

public:
    CleanStatus getCleanStatus() {
        return clean_status;
    }

    void setCleanStatus(CleanStatus cleanStatus) {
        clean_status = cleanStatus;
    }
};

}}}

#endif // CLEANABLE_HPP_INCLUDED
