#ifndef CHOMSKY_TEST_HPP_INCLUDED
#define CHOMSKY_TEST_HPP_INCLUDED

#include "grammar.hpp"
#include "production-rule.hpp"
#include "symbol-type.hpp"

#include "chomsky_type.hpp"

namespace echelon { namespace grammar_utilities {

template<typename T>
class ChomskyTest {
public:
    static bool isTypeZero(const Grammar<T> *const grammar) {
        return grammar->isValid();
    }

    static bool isTypeOne(const Grammar<T> *const grammar) {
        if (!ChomskyTest::isTypeZero(grammar)) {
            return false;
        }

        bool hasNonMonotonicRule = false;
        grammar->eachRule([&hasNonMonotonicRule](auto *production_rule) {
            if (production_rule->keyLength() > production_rule->valueLength()) {
                hasNonMonotonicRule = true;
            }
        });

        return !hasNonMonotonicRule;
    }

    static bool isTypeTwo(const Grammar<T> *const grammar) {
        if(!ChomskyTest::isTypeOne(grammar)) {
            return false;
        }

        bool hasRuleWithKeyOtherThanSingleTerminal = false;
        grammar->eachRule([&hasRuleWithKeyOtherThanSingleTerminal](auto *production_rule) {
            if (production_rule->keyLength() != 1 || production_rule->getFirstKeySymbol()->getType() != SymbolType::NonTerminal) {
                hasRuleWithKeyOtherThanSingleTerminal = true;
            }
        });

        return !hasRuleWithKeyOtherThanSingleTerminal;
    }

    static bool isTypeThree(const Grammar<T> *const grammar) {
        if (!ChomskyTest::isTypeTwo(grammar)) {
            return false;
        }

        bool hasInvalidTypeThreeRule = false;
        grammar->eachRule([&hasInvalidTypeThreeRule](auto *production_rule) {
            int terminalSymbolCount = 0;
            production_rule->eachValueSymbol([&terminalSymbolCount](auto *symbol) {
                if (symbol->getType() == SymbolType::Terminal) {
                    ++terminalSymbolCount;
                }
            });

            int nonTerminalSymbolsInLastPosition = production_rule->valueLength() > 0 && production_rule->getLastValueSymbol()->getType() == SymbolType::NonTerminal ? 1 : 0;

            if (terminalSymbolCount + nonTerminalSymbolsInLastPosition != production_rule->valueLength()) {
                hasInvalidTypeThreeRule = true;
            }
        });

        return !hasInvalidTypeThreeRule;
    }
};

}}

#endif // CHOMSKY_TEST_HPP_INCLUDED
