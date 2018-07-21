#ifndef UNGER_PARSER_HPP_INCLUDED
#define UNGER_PARSER_HPP_INCLUDED

#include "input-sequence.hpp"
#include "chomsky-test.hpp"

namespace echelon { namespace parsing { namespace impl { namespace unger {

template<typename T>
class UngerParser {
public:
    void parse(echelon::grammar_model::Grammar<T>* grammar, echelon::parsing::api::InputSequence<T>* input_sequence, ) {
        if (!ChomskyTest::isContextFree(grammar)) {
            // This parsing technique requires a context free grammar or stricter.
            return;
        }

        
    }
};

}}}}

#endif // UNGER_PARSER_HPP_INCLUDED