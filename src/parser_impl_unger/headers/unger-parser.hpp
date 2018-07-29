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

        std::cout << "Starting parse." << std::endl;

        if (!ChomskyTest::isContextFree(grammar)) {
            // This parsing technique requires a context free grammar or stricter.
            std::cout << "Not a context free grammar" << std::endl;
            return nullptr;
        }

        std::shared_ptr<Symbol> start_symbol(grammar->getStartSymbol());
        return _parse(input_sequence, start_symbol);
    }

private:
    std::shared_ptr<echelon::parsing::api::ParseTree> _parse(echelon::parsing::api::InputSequence<T>* input_sequence, std::shared_ptr<Symbol> current_symbol, unsigned depth = 0) {
        using namespace echelon::parsing::api;

        if (depth > 0) {
            return nullptr;
        }

        std::cout << "Top of loop with sequence [";
        if (input_sequence == nullptr) {
            std::cout << "nullptr";
        }
        else {
            input_sequence->each([](auto element) {
                std::cout << element;
            });
        }
        std::cout << "]" << std::endl;

        std::cout << "The current symbol is [";
        if (current_symbol->getType() == SymbolType::Terminal) {
            std::cout << std::static_pointer_cast<TerminalSymbol<char>>(current_symbol)->getValue();
        }
        else {
            std::cout << std::static_pointer_cast<NonTerminalSymbol<std::string>>(current_symbol)->getValue();
        }
        std::cout << "]" << std::endl;

        std::shared_ptr<ParseTree> local_root = nullptr;

        grammar->eachRule([this, &input_sequence, &current_symbol, &local_root, &depth](auto production_rule) {
            if (local_root != nullptr) {
                // TODO Because of the eachRule the iteration cannot be stopped when a match has been found
                std::cout << "Aleady finished matching. Skip loop." << std::endl;
                return;
            }

            if (!production_rule->getFirstKeySymbol()->equals(current_symbol)) {
                std::cout << "Filtering production rule which doesn't match" << std::endl;
                return;
            }

            auto value_length = production_rule->valueLength();
            std::cout << "Production rule has [" << value_length << "] symbols" << std::endl;
            
            if (value_length == 1) {
                std::cout << "Special case for value length 1" << std::endl;

                auto symbol = production_rule->getValues()[0];
                if (symbol->getType() == SymbolType::Terminal) {
                    if (input_sequence == nullptr) {
                        if (std::static_pointer_cast<TerminalSymbol<T>>(symbol)->isEmpty()) {
                            local_root = ParseTree::createWithSymbol(current_symbol);
                            local_root->addChild(symbol);
                        }
                    }
                    else if (input_sequence->matches(std::static_pointer_cast<TerminalSymbol<T>>(symbol)->getValue())) {
                        local_root = ParseTree::createWithSymbol(current_symbol);
                        local_root->addChild(symbol);
                    }
                }

                return;
            }

            std::cout << "Going to build partition generator." << std::endl;
            internal::SequencePartitionGenerator<T> partitionGenerator(input_sequence, value_length);
            std::cout << "Partition generator ready." << std::endl;
            while (partitionGenerator.currentValue() != nullptr) {
                std::cout << "Built partition: [";
                auto indices = partitionGenerator.currentValue()->getPartitionIndices();
                for (int i = 0; i < indices.size(); i++) {
                    std::cout << indices[i];
                    if (i + 1 != indices.size()) {
                        std::cout << " | ";
                    }
                }
                std::cout << "]" << std::endl;

                // TODO rename partition indices, misleading name.
                auto partition = partitionGenerator.currentValue()->getPartitionIndices();

                std::list<std::shared_ptr<ParseTree>> potential_nodes;

                bool partition_success = true;
                unsigned start_index = 0;
                for (int i = 0; i < partition.size(); i++) {
                    std::cout << "Ready to create subsequence starting at [" << start_index << "] with length [" << partition[i] << "]" << std::endl;
                    auto sub_input_sequence = input_sequence == nullptr ? nullptr : input_sequence->getSubSequence(start_index, partition[i]);

                    std::cout << "Working with sub-sequence [";
                    if (sub_input_sequence == nullptr) {
                        std::cout << "nullptr";
                    }
                    else {
                        sub_input_sequence->each([](auto element) {
                            std::cout << element;
                        });
                    }
                    std::cout << "]" << std::endl;
                    
                    auto symbol = production_rule->getValues()[i];
                    if (symbol->getType() == SymbolType::Terminal) {
                        std::cout << "Processing terminal" << std::endl;

                        // Either a leaf node or the match has failed.
                        if (sub_input_sequence == nullptr) {
                            if (std::static_pointer_cast<TerminalSymbol<T>>(symbol)->isEmpty()) {
                                potential_nodes.push_back(ParseTree::createWithSymbol(symbol));
                            }
                            else {
                                partition_success = false;
                                break;
                            }
                        }
                        else if (sub_input_sequence->matches(std::static_pointer_cast<TerminalSymbol<T>>(symbol)->getValue())) {
                            potential_nodes.push_back(ParseTree::createWithSymbol(symbol));
                        }
                        else {
                            partition_success = false;
                            break;
                        }
                    }
                    else if (symbol->getType() == SymbolType::NonTerminal) {
                        std::cout << "Processing non-terminal." << std::endl;

                        // Recursively try to parse the substring based on this non-terminal
                        auto sub_tree = _parse(sub_input_sequence, symbol, depth + 1);
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