#ifndef GRAMMAR_CLEANER_HPP_INCLUDED
#define GRAMMAR_CLEANER_HPP_INCLUDED

#include <map>
#include <list>

#include "grammar.hpp"
#include "production-rule.hpp"
#include "symbol.hpp"
#include "symbol-type.hpp"

#include "reason.hpp"

#include "chomsky-test.hpp"

#include "clean-status.hpp"

namespace echelon { namespace grammar_utilities { namespace cleaning {

class GrammarCleaner {

    void applyCleaning(
        Grammar *grammar, 
        const std::map<ProductionRule*, CleanStatus> &rule_status
    ) {
        for (auto& rs_pair : rule_status) {
            if (rs_pair.second == CleanStatus::IsNonProductive) {
                grammar->removeProductionRule(rs_pair.first);
            }
        }
    }

    void applyCleaning(
        Grammar *grammar, 
        const std::map<Symbol*, CleanStatus> &non_terminal_status
    ) {
        std::list<ProductionRule*> rules_to_remove;
        grammar->eachRule([&rules_to_remove, &non_terminal_status](ProductionRule *rule) {
            if (non_terminal_status.at(rule->getFirstKeySymbol()) == CleanStatus::IsNotReachable) {
                rules_to_remove.push_front(rule);
            }
        });

        for (auto rule : rules_to_remove) {
            grammar->removeProductionRule(rule);
        }
    }

    bool removeNonProductiveRules(Grammar *grammar, echelon::diagnostics::Reason *reason) {
        if (!ChomskyTest::isContextFree(grammar)) {
            reason->addReason("Do not know how to clean the grammar because it is not context free");
            return false;
        }

        std::map<ProductionRule*, CleanStatus> rule_status;
        grammar->eachRule([&rule_status](ProductionRule *rule) {
            rule_status[rule] = CleanStatus::DoNotKnow;
        });

        std::map<Symbol*, CleanStatus> non_terminal_status;
        grammar->eachNonTerminal([&non_terminal_status](Symbol *non_terminal) {
            non_terminal_status[non_terminal] = CleanStatus::DoNotKnow;
        });

        // Initialised so the first pass will run.
        int numberUpdatedDuringPass = -1;
        while (numberUpdatedDuringPass != 0) {
            numberUpdatedDuringPass = 0;

            for (auto& rs_pair : rule_status) {
                if (rs_pair.second != CleanStatus::DoNotKnow) {
                    continue;
                }

                int numberUnknown = rs_pair.first->valueLength();
                rs_pair.first->eachValueSymbol([&numberUnknown, &non_terminal_status](Symbol *symbol) {
                    if (symbol->getType() == SymbolType::Terminal || non_terminal_status[symbol] == CleanStatus::IsProductive) {
                        --numberUnknown;
                    }
                });

                if (numberUnknown == 0) {
                    // This pass found something new, so another pass should be run.
                    ++numberUpdatedDuringPass;

                    // Mark this rule as productive.
                    rs_pair.second = CleanStatus::IsProductive;

                    // Mark the non-terminal on the left as productive.
                    // Note that because we've checked for FS grammar this is the only key symbol.
                    non_terminal_status[rs_pair.first->getFirstKeySymbol()] = CleanStatus::IsProductive;
                }
            }
        }

        for (auto& rs_pair : rule_status) {
            if (rs_pair.second == CleanStatus::DoNotKnow) {
                rs_pair.second = CleanStatus::IsNonProductive;
            }
        }

        for (auto& nts_pair : non_terminal_status) {
            if (nts_pair.second == CleanStatus::DoNotKnow) {
                nts_pair.second = CleanStatus::IsNonProductive;
            }
        }

        applyCleaning(grammar, rule_status);

        return true;
    }

    bool removeUnreachableNonTerminals(Grammar *grammar, echelon::diagnostics::Reason *reason) {
        std::map<Symbol*, CleanStatus> non_terminal_status;
        grammar->eachNonTerminal([&non_terminal_status](Symbol *non_terminal) {
            non_terminal_status[non_terminal] = CleanStatus::DoNotKnow;
        });

        // optimisation only, not required.
        std::map<ProductionRule*, CleanStatus> production_rules;
        grammar->eachRule([&production_rules](ProductionRule *rule) {
            production_rules[rule] = CleanStatus::DoNotKnow;
        });

        // Initialise the search
        non_terminal_status[grammar->getStartSymbol()] = CleanStatus::IsReachable;

        // Initialised so at least one pass will run.
        int number_of_symbols_marked = 1;
        while (number_of_symbols_marked != 0) {
            number_of_symbols_marked = 0;

            grammar->eachRule([&non_terminal_status, &production_rules, &number_of_symbols_marked](ProductionRule *rule) {
                if (production_rules[rule] != CleanStatus::DoNotKnow) {
                    return;
                }

                if (non_terminal_status[rule->getFirstKeySymbol()] == CleanStatus::IsReachable) {
                    // Don't test this rule again.
                    production_rules[rule] = CleanStatus::IsReachable;

                    rule->eachValueSymbol([&non_terminal_status, &number_of_symbols_marked](Symbol* symbol) {
                        if (symbol->getType() == SymbolType::NonTerminal) {
                            if (non_terminal_status[symbol] == CleanStatus::DoNotKnow) {
                                non_terminal_status[symbol] = CleanStatus::IsReachable;
                                ++number_of_symbols_marked;
                            }
                        }
                    });
                }
            });
        }
        
        for (auto& nts_pair : non_terminal_status) {
            if (nts_pair.second == CleanStatus::DoNotKnow) {
                nts_pair.second = CleanStatus::IsNotReachable;
            }
        }

        applyCleaning(grammar, non_terminal_status);

        return true;
    }
public:
    bool clean(Grammar *grammar, echelon::diagnostics::Reason *reason) {
        if (!removeNonProductiveRules(grammar, reason)) {
            return false;
        }
        
        return removeUnreachableNonTerminals(grammar, reason);
    }
};

}}}

#endif // GRAMMAR_CLEANER_HPP_INCLUDED
