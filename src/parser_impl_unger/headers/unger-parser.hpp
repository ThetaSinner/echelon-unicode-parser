#ifndef UNGER_PARSER_HPP_INCLUDED
#define UNGER_PARSER_HPP_INCLUDED

#include <stdexcept>
#include <list>
#include <algorithm>

#include "input-sequence.hpp"
#include "chomsky-test.hpp"
#include "parse-tree.hpp"
#include "sequence-partition-generator.hpp"
#include "symbol-type.hpp"

namespace echelon { namespace parsing { namespace impl { namespace unger {

template<typename T>
class UngerParser {
    echelon::grammar_model::Grammar* grammar;
public:
    void setGrammar(echelon::grammar_model::Grammar* grammar) {
        this->grammar = grammar;
    }

    std::shared_ptr<echelon::parsing::api::ParseTree> parse(echelon::parsing::api::InputSequence<T>* input_sequence) {
        using namespace echelon::parsing::api;
        using namespace echelon::grammar_utilities;

        if (!ChomskyTest::isContextFree(grammar)) {
            // This parsing technique requires a context free grammar or stricter.
            return nullptr;
        }

        std::shared_ptr<Symbol> start_symbol(grammar->getStartSymbol());
        return _parse(input_sequence, start_symbol);
    }

private:
    std::shared_ptr<echelon::parsing::api::ParseTree> _parse(echelon::parsing::api::InputSequence<T>* input_sequence, std::shared_ptr<Symbol> current_symbol) {
        using namespace echelon::parsing::api;

        std::shared_ptr<ParseTree> local_root = nullptr;

        grammar->eachRule([this, &input_sequence, &current_symbol, &local_root](auto production_rule) {
            if (local_root != nullptr) {
                // TODO Because of the eachRule the iteration cannot be stopped when a match has been found
                return;
            }

            if (!production_rule->getFirstKeySymbol()->equals(current_symbol)) {
                return;
            }

            auto valueLength = production_rule->valueLength();
            
            // TODO actually need to check the value length before trying to construct
            // Only need to check length greater than 1 otherwise can just do termination check.
            internal::SequencePartitionGenerator partitionGenerator(input_sequence, valueLength);
            while (partitionGenerator.currentValue() != nullptr) {
                // TODO rename partition indices, misleading name.
                auto partition = partitionGenerator.currentValue()->getPartitionIndices();

                std::list<std::shared_ptr<ParseTree>> potential_nodes;

                bool partition_success = true;
                unsigned start_index = 0;
                for (int i = 0; i < partition.size(); i++) {
                    auto sub_input_sequence = input_sequence->getSubSequence(start_index, partition[i]);
                    
                    auto symbol = production_rule->getValues()[i];
                    if (symbol->getType() == SymbolType::Terminal) {
                        // Either a leaf node or the match has failed.
                        if (sub_input_sequence->matches(std::static_pointer_cast<TerminalSymbol<T>>(symbol)->getValue())) {
                            potential_nodes.push_back(ParseTree::createWithSymbol(symbol));
                        }
                        else {
                            partition_success = false;
                            break;
                        }
                    }
                    else if (symbol->getType() == SymbolType::NonTerminal) {
                        auto sub_tree = _parse(sub_input_sequence, symbol);
                        if (sub_tree == nullptr) {
                            partition_success = false;
                            break;
                        }
                        else {
                            potential_nodes.push_back(ParseTree::createWithSymbol(symbol));
                        }
                    }
                    else {
                        throw new std::domain_error("Invalid symbol");
                    }

                    start_index += partition[i];
                }

                if (partition_success) {
                    local_root = ParseTree::createWithSymbol(current_symbol);
                    std::for_each(potential_nodes.begin(), potential_nodes.end(), [&local_root](const std::shared_ptr<ParseTree>& node) {
                        local_root->addSubTree(node);
                    });

                    break;
                }

                partitionGenerator.moveNext();
            }
        });

        return local_root;
    }
};

}}}}

#endif // UNGER_PARSER_HPP_INCLUDED