#ifndef CLEAN_STATUS_HPP_INCLUDED
#define CLEAN_STATUS_HPP_INCLUDED

namespace echelon { namespace grammar_utilities { namespace cleaning {

enum class CleanStatus {
    IsProductive,
    IsNonProductive,
    DoNotKnow
};

}}}

#endif // CLEAN_STATUS_HPP_INCLUDED
