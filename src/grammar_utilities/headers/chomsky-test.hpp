#ifndef CHOMSKY_TEST_HPP_INCLUDED
#define CHOMSKY_TEST_HPP_INCLUDED

#include "grammar.hpp"
#include "production-rule.hpp"
#include "symbol-type.hpp"

#include "chomsky-type.hpp"

namespace echelon { namespace grammar_utilities {

using namespace echelon::grammar_model;

class ChomskyTest {
public:
    static bool isTypeZero(const Grammar *const grammar) {
        return grammar->isValid(nullptr);
    }

    static bool isTypeOne(const Grammar *const grammar) {
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

    static bool isTypeTwo(const Grammar *const grammar) {
        if(!ChomskyTest::isTypeOne(grammar)) {
            return false;
        }

        bool hasRuleWithKeyOtherThanSingleNonTerminal = false;
        grammar->eachRule([&hasRuleWithKeyOtherThanSingleNonTerminal](auto *production_rule) {
            if (production_rule->keyLength() != 1 || production_rule->getFirstKeySymbol()->getType() != SymbolType::NonTerminal) {
                hasRuleWithKeyOtherThanSingleNonTerminal = true;
            }
        });

        return !hasRuleWithKeyOtherThanSingleNonTerminal;
    }

    static bool isTypeThree(const Grammar *const grammar) {
        if (!ChomskyTest::isTypeTwo(grammar)) {
            return false;
        }

        bool hasInvalidTypeThreeRule = false;
        grammar->eachRule([&hasInvalidTypeThreeRule](auto *production_rule) {
            unsigned terminalSymbolCount = 0;
            production_rule->eachValueSymbol([&terminalSymbolCount](auto *symbol) {
                if (symbol->getType() == SymbolType::Terminal) {
                    ++terminalSymbolCount;
                }
            });

            unsigned nonTerminalSymbolsInLastPosition = production_rule->valueLength() > 0 && production_rule->getLastValueSymbol()->getType() == SymbolType::NonTerminal ? 1 : 0;

            if (terminalSymbolCount + nonTerminalSymbolsInLastPosition != production_rule->valueLength()) {
                hasInvalidTypeThreeRule = true;
            }
        });

        return !hasInvalidTypeThreeRule;
    }

    static bool isContextSensitive(const Grammar *const grammar) {
        return isTypeOne(grammar);
    }

    static bool isContextFree(const Grammar *const grammar) {
        return isTypeTwo(grammar);
    }

    static bool isRegular(const Grammar *const grammar) {
        return isTypeThree(grammar);
    }
};

}}

#endif // CHOMSKY_TEST_HPP_INCLUDED
