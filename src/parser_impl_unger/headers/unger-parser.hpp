#ifndef UNGER_PARSER_HPP_INCLUDED
#define UNGER_PARSER_HPP_INCLUDED

#include "input-sequence.hpp"
#include "chomsky-test.hpp"
#include "parse-tree.hpp"

namespace echelon { namespace parsing { namespace impl { namespace unger {

template<typename T>
class UngerParser {
public:
    void parse(echelon::grammar_model::Grammar* grammar, echelon::parsing::api::InputSequence<T>* input_sequence) {
        using namespace echelon::parsing::api;
        using namespace echelon::grammar_utilities;

        if (!ChomskyTest::isContextFree(grammar)) {
            // This parsing technique requires a context free grammar or stricter.
            return;
        }

        std::shared_ptr<Symbol> start_symbol(grammar->getStartSymbol());
        ParseTree::createWithSymbol(start_symbol);
    }
};

}}}}

#endif // UNGER_PARSER_HPP_INCLUDED