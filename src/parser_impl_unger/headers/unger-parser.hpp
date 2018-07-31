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
class ParseQuestion {
    echelon::parsing::api::InputSequence<T>* input_sequence;
    std::shared_ptr<Symbol> symbol;
public:
    ParseQuestion(echelon::parsing::api::InputSequence<T>* input_sequence, std::shared_ptr<Symbol> symbol) {
        this->input_sequence = input_sequence;
        this->symbol = symbol;
    }

    bool matches(echelon::parsing::api::InputSequence<T>* input_sequence, std::shared_ptr<Symbol> symbol) {
		if (this->input_sequence == nullptr || input_sequence == nullptr) {
			return this->input_sequence == nullptr && input_sequence == nullptr;
		}
        return *(this->input_sequence) == *input_sequence && *(this->symbol) == *symbol;
    }
};

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

        std::vector<ParseQuestion<T>> question_list;

        std::shared_ptr<Symbol> start_symbol(grammar->getStartSymbol());
		auto result = _parse(input_sequence, start_symbol, question_list);

		if (result.size() == 0) {
			return nullptr;
		}
		else {
			auto root = ParseTree::createWithSymbol(start_symbol);
			for (const auto& node : result) {
				root->addSubTree(node);
			}
			return root;
		}
    }

private:
    std::list<std::shared_ptr<echelon::parsing::api::ParseTree>> _parse(echelon::parsing::api::InputSequence<T>* input_sequence, std::shared_ptr<Symbol> current_symbol, std::vector<ParseQuestion<T>>& question_list) {
        using namespace echelon::parsing::api;
		
		/*
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
		*/

		/*
        std::cout << "The current symbol is [";
        if (current_symbol->getType() == SymbolType::Terminal) {
            std::cout << std::static_pointer_cast<TerminalSymbol<char>>(current_symbol)->getValue();
        }
        else {
            std::cout << std::static_pointer_cast<NonTerminalSymbol<std::string>>(current_symbol)->getValue();
        }
        std::cout << "]" << std::endl;
		*/

        std::list<std::shared_ptr<echelon::parsing::api::ParseTree>> result;

        grammar->eachRule([this, &input_sequence, &current_symbol, &result, &question_list](auto production_rule) {
            if (result.size() != 0) {
                // TODO Because of the eachRule the iteration cannot be stopped when a match has been found
                // std::cout << "Aleady finished matching. Skip loop." << std::endl;
                return;
            }

            if (!production_rule->getFirstKeySymbol()->equals(current_symbol)) {
                // std::cout << "Filtering production rule which doesn't match" << std::endl;
                return;
            }

            auto value_length = production_rule->valueLength();
            // std::cout << "Production rule has [" << value_length << "] symbols" << std::endl;
            
            if (value_length == 1) {
                // std::cout << "Special case for value length 1" << std::endl;

                auto symbol = production_rule->getValues()[0];
                if (symbol->getType() == SymbolType::Terminal) {
                    if (input_sequence == nullptr) {
                        if (std::static_pointer_cast<TerminalSymbol<T>>(symbol)->isEmpty()) {
                            result.push_back(ParseTree::createWithSymbol(symbol));
                        }
                    }
                    else if (input_sequence->matches(std::static_pointer_cast<TerminalSymbol<T>>(symbol)->getValue())) {
                        result.push_back(ParseTree::createWithSymbol(symbol));
                    }
                }
				else if (symbol->getType() == SymbolType::NonTerminal) {
					bool prune = false;
                    for (int i = 0; i < question_list.size(); i++) {
                        if ((question_list[i]).matches(input_sequence, symbol)) {
                            prune = true;
                            break;
                        }
                    }

                    if (prune) {
                        return;
                    }

                    ParseQuestion<T> next_question(input_sequence, symbol);
                    question_list.push_back(next_question);
                    // Recursively try to parse the substring based on this non-terminal
                    auto sub_tree_list = _parse(input_sequence, symbol, question_list);
                    question_list.pop_back();
                    if (sub_tree_list.size() == 0) {
                        return;
                    }
                    else {
                        auto local_result = ParseTree::createWithSymbol(symbol);
						for (const auto& node : sub_tree_list) {
							local_result->addSubTree(node);
						}

						result.push_back(local_result);
                    }
				}
				else {
					throw new std::domain_error("Invalid symbol");
				}

                return;
            }

            // std::cout << "Going to build partition generator." << std::endl;
            internal::SequencePartitionGenerator<T> partitionGenerator(input_sequence, value_length);
            // std::cout << "Partition generator ready." << std::endl;
            while (partitionGenerator.currentValue() != nullptr) {
                /*
				std::cout << "Built partition: [";
                auto indices = partitionGenerator.currentValue()->getPartitionIndices();
                for (int i = 0; i < indices.size(); i++) {
                    std::cout << indices[i];
                    if (i + 1 != indices.size()) {
                        std::cout << " | ";
                    }
                }
                std::cout << "]" << std::endl;
				*/

                // TODO rename partition indices, misleading name.
                auto partition = partitionGenerator.currentValue()->getPartitionIndices();

                std::list<std::shared_ptr<ParseTree>> potential_nodes;

                bool partition_success = true;
                unsigned start_index = 0;
                for (int i = 0; i < partition.size(); i++) {
                    // std::cout << "Ready to create subsequence starting at [" << start_index << "] with length [" << partition[i] << "]" << std::endl;
                    auto sub_input_sequence = input_sequence == nullptr ? nullptr : input_sequence->getSubSequence(start_index, partition[i]);

					/*
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
                    */

                    auto symbol = production_rule->getValues()[i];
                    if (symbol->getType() == SymbolType::Terminal) {
                        // std::cout << "Processing terminal" << std::endl;

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
                        // std::cout << "Processing non-terminal." << std::endl;

                        bool prune = false;
                        for (int i = 0; i < question_list.size(); i++) {
                            if ((question_list[i]).matches(sub_input_sequence, symbol)) {
                                prune = true;
                                break;
                            }
                        }

                        if (prune) {
                            partition_success = false;
                            break;
                        }

                        ParseQuestion<T> next_question(sub_input_sequence, symbol);
                        question_list.push_back(next_question);
                        // Recursively try to parse the substring based on this non-terminal
                        auto sub_tree_list = _parse(sub_input_sequence, symbol, question_list);
                        question_list.pop_back();
                        if (sub_tree_list.size() == 0) {
                            partition_success = false;
                            break;
                        }
                        else {
							auto root = ParseTree::createWithSymbol(symbol);
							for (const auto& node : sub_tree_list) {
								root->addSubTree(node);
							}

                            potential_nodes.push_back(root);
                        }
                    }
                    else {
                        throw new std::domain_error("Invalid symbol");
                    }

                    start_index += partition[i];
                }

                if (partition_success) {
                    std::for_each(potential_nodes.begin(), potential_nodes.end(), [&result](const std::shared_ptr<ParseTree>& node) {
                        result.push_back(node);
                    });

                    break;
                }

                partitionGenerator.moveNext();
            }
        });

        return result;
    }
};

}}}}

#endif // UNGER_PARSER_HPP_INCLUDED