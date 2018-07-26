#ifndef UNGER_PARSER_HPP_INCLUDED
#define UNGER_PARSER_HPP_INCLUDED

#include "input-sequence.hpp"
#include "chomsky-test.hpp"
#include "parse-tree.hpp"
#include "sequence-partition-generator.hpp"

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

        grammar->eachRule([&start_symbol, &input_sequence](auto production_rule) {
            if (!production_rule->getFirstKeySymbol()->equals(start_symbol)) {
                return;
            }

            auto valueLength = production_rule->valueLength();
            
            internal::SequencePartitionGenerator partitionGenerator(input_sequence, valueLength);
            while (partitionGenerator.currentValue() != nullptr) {
                auto partition = partitionGenerator.currentValue()->getPartitionIndices();

                unsigned start_index = 0;
                for (int i = 0; i < partition.size(); i++) {
                    auto sub_input_sequence = input_sequence->getSubSequence(start_index, partition[i]);
                    // TODO recurse.
                    start_index += partition[i];
                }

                partitionGenerator.moveNext();
            }    
        });
    }
};

}}}}

#endif // UNGER_PARSER_HPP_INCLUDED