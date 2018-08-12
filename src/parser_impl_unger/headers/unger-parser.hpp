#ifndef UNGER_PARSER_HPP_INCLUDED
#define UNGER_PARSER_HPP_INCLUDED

#include <stdexcept>
#include <list>
#include <algorithm>
#include <map>

#include "input-sequence.hpp"
#include "chomsky-test.hpp"
#include "parse-tree.hpp"
#include "sequence-partition-generator.hpp"
#include "symbol-type.hpp"
#include "terminal-symbol.hpp"

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

    virtual bool operator<(const ParseQuestion<T>& parse_question) const {
        if (input_sequence == nullptr || parse_question.input_sequence == nullptr) {
			if (input_sequence == nullptr && parse_question.input_sequence != nullptr) {
				return true;
			}
			else if (input_sequence != nullptr && parse_question.input_sequence == nullptr) {
				return false;
			}

            return symbol->getId() < parse_question.symbol->getId();
        }

        if ((*input_sequence) < (* parse_question.input_sequence)) {
            return true;
        }
        else if (!((* parse_question.input_sequence) < (*input_sequence))) {
            return symbol->getId() < parse_question.symbol->getId();
        }
        else {
            return false;
        }
    }
};

template<typename T>
struct InputSequenceComparator {
  bool operator() (const ParseQuestion<T>* lhs, const ParseQuestion<T>* rhs) const
  { return (*lhs) < (*rhs); }
};

template<typename T>
using QuestionCache = std::map<ParseQuestion<T>*, std::list<std::shared_ptr<echelon::parsing::api::ParseTree>>, InputSequenceComparator<T>>;

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
            std::cout << "Not a context free grammar" << std::endl;
            return nullptr;
        }

        std::vector<ParseQuestion<T>> question_list;
        QuestionCache<T> question_cache;

        std::shared_ptr<Symbol> start_symbol(grammar->getStartSymbol());
		auto result = _parse(input_sequence, start_symbol, question_list, question_cache);

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
    // The question_list is used to track the questions that are currently being asked to prevent infinite recursion.
    // The question_cache is used to remember questions which already have answered. This is done for performance.
    std::list<std::shared_ptr<echelon::parsing::api::ParseTree>> _parse(echelon::parsing::api::InputSequence<T>* input_sequence, std::shared_ptr<Symbol> current_symbol, std::vector<ParseQuestion<T>>& question_list, QuestionCache<T>& question_cache) {
        using namespace echelon::parsing::api;

        std::list<std::shared_ptr<echelon::parsing::api::ParseTree>> result;

        grammar->eachRule([this, &input_sequence, &current_symbol, &result, &question_list, &question_cache](auto production_rule) {
            if (result.size() != 0) {
                // TODO Because of the eachRule the iteration cannot be stopped when a match has been found
                return;
            }

            if (!production_rule->getFirstKeySymbol()->equals(current_symbol)) {
                return;
            }

            auto value_length = production_rule->valueLength();
            
            if (value_length == 1) {

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
                    for (std::size_t i = 0; i < question_list.size(); i++) {
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
                    auto sub_tree_list = _parse(input_sequence, symbol, question_list, question_cache);
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

            internal::SequencePartitionGenerator<T> partitionGenerator(input_sequence, value_length);
            while (partitionGenerator.currentValue() != nullptr) {
				std::vector<InputSequence<T>*> sub_sequences;

				bool skipPartition = false;
				unsigned start_index = 0;
                // TODO rename partition indices, misleading name.
                auto partition = partitionGenerator.currentValue()->getPartitionIndices();
				for (std::size_t i = 0; i < partition.size(); i++) {
					auto sub_input_sequence = input_sequence == nullptr ? nullptr : input_sequence->getSubSequence(start_index, partition[i]);

					auto symbol = production_rule->getValues()[i];
					if (sub_input_sequence == nullptr) {
						// If the sequence is empty and it needs to match a non-empty terminal then it cannot succeed, skip.
						skipPartition = symbol->getType() == SymbolType::Terminal && !std::static_pointer_cast<TerminalSymbol<T>>(symbol)->isEmpty();
					}
					else {
						// If the sequence is non-empty it must have length >= 1, so can only be matched by a single terminal non-empty terminal if the sequence has length 1.
						skipPartition = symbol->getType() == SymbolType::Terminal && (sub_input_sequence->length() != 1 || std::static_pointer_cast<TerminalSymbol<T>>(symbol)->isEmpty());
					}

					if (skipPartition) {
						break;
					}

					sub_sequences.push_back(sub_input_sequence);
					start_index += partition[i];
				}

				if (skipPartition) {
					partitionGenerator.moveNext();
					continue;
				}

                std::list<std::shared_ptr<ParseTree>> potential_nodes;

                bool partition_success = true;
                for (std::size_t i = 0; i < sub_sequences.size(); i++) {
                    auto sub_input_sequence = sub_sequences[i];

                    auto symbol = production_rule->getValues()[i];
                    if (symbol->getType() == SymbolType::Terminal) {

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
                        bool prune = false;
                        for (unsigned i = 0; i < question_list.size(); i++) {
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

                        std::list<std::shared_ptr<echelon::parsing::api::ParseTree>> sub_tree_list;
                        auto cache_question = new ParseQuestion<T>(sub_input_sequence, symbol);
                        if (question_cache.find(cache_question) != question_cache.end()) {
                            sub_tree_list = question_cache[cache_question];
                        }
                        else {
                            // Recursively try to parse the substring based on this non-terminal
                            sub_tree_list = _parse(sub_input_sequence, symbol, question_list, question_cache);
                            question_cache[cache_question] = sub_tree_list;
                        }

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