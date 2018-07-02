#ifndef GRAMMAR_CLEANER_HPP_INCLUDED
#define GRAMMAR_CLEANER_HPP_INCLUDED

#include <map>

#include "grammar.hpp"
#include "production-rule.hpp"
#include "symbol.hpp"
#include "symbol-type.hpp"

#include "reason.hpp"

#include "chomsky-test.hpp"

#include "clean-status.hpp"

namespace echelon { namespace grammar_utilities { namespace cleaning {

template<typename T>
class GrammarCleaner {

    void applyCleaning(
        Grammar<T> *grammar, 
        const std::map<ProductionRule<T>*, CleanStatus> &rule_status
    ) {
        for (auto& rs_pair : rule_status) {
            if (rs_pair.second == CleanStatus::IsNonProductive) {
                grammar->removeProductionRule(rs_pair.first);
            }
        }
    }
public:
    bool clean(Grammar<T> *grammar, Reason *reason) {
        if (!ChomskyTest<std::string>::isContextFree(grammar)) {
            reason->addReason("Do not know how to clean the grammar because it is not context free");
            return false;
        }

        std::map<ProductionRule<T>*, CleanStatus> rule_status;
        grammar->eachRule([&rule_status](ProductionRule<T> *rule) {
            rule_status[rule] = CleanStatus::DoNotKnow;
        });

        std::map<Symbol<T>*, CleanStatus> non_termial_status;
        grammar->eachNonTerminal([&non_termial_status](Symbol<T> *non_terminal) {
            non_termial_status[non_terminal] = CleanStatus::DoNotKnow;
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
                rs_pair.first->eachValueSymbol([&numberUnknown, &non_termial_status](Symbol<T> *symbol) {
                    if (symbol->getType() == SymbolType::Terminal || non_termial_status[symbol] == CleanStatus::IsProductive) {
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
                    non_termial_status[rs_pair.first->getFirstKeySymbol()] = CleanStatus::IsProductive;
                }
            }
        }

        for (auto& rs_pair : rule_status) {
            if (rs_pair.second == CleanStatus::DoNotKnow) {
                rs_pair.second = CleanStatus::IsNonProductive;
            }
        }

        for (auto& nts_pair : non_termial_status) {
            if (nts_pair.second == CleanStatus::DoNotKnow) {
                nts_pair.second = CleanStatus::IsNonProductive;
            }
        }

        applyCleaning(grammar, rule_status);
    }
};

}}}

#endif // GRAMMAR_CLEANER_HPP_INCLUDED
