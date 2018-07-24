#ifndef GRAMMAR_HPP_INCLUDED
#define GRAMMAR_HPP_INCLUDED

#include <vector>
#include <algorithm>
#include <functional>
#include <sstream>

#include "reason.hpp"

#include "symbol.hpp"
#include "production-rule.hpp"

namespace echelon { namespace grammar_model {

class Grammar {
    std::vector<Symbol*> terminals;
    std::vector<Symbol*> non_terminals;
    Symbol* start_symbol;
    std::vector<ProductionRule*> production_rules;

public:
    void addTerminal(Symbol* terminal) {
        terminals.push_back(terminal);
    }

    void addNonTerminal(Symbol* non_terminal) {
        non_terminals.push_back(non_terminal);
    }

    void setStartSymbol(Symbol* start_symbol) {
        this->start_symbol = start_symbol;
    }

    Symbol* getStartSymbol() {
        return start_symbol;
    }

    void addProductionRule(ProductionRule* production_rule) {
        production_rules.push_back(production_rule);
    }

    ProductionRule* getProductionRule(unsigned index) {
        return production_rules[index];
    }

    unsigned numberOfProductionRules() {
        return production_rules.size();
    }

    void eachRule(std::function<void(ProductionRule*)> action) const {
        std::for_each(production_rules.begin(), production_rules.end(), action);
    }

    void eachNonTerminal(std::function<void(Symbol*)> action) const {
        std::for_each(non_terminals.begin(), non_terminals.end(), action);
    }

    void removeProductionRule(ProductionRule* production_rule) {        
        production_rules.erase(std::remove(production_rules.begin(), production_rules.end(), production_rule), production_rules.end());
    }

    bool isValid(echelon::diagnostics::Reason *reason) const {
        if (start_symbol == nullptr) {
            if (terminals.size() == 0 && non_terminals.size() == 0 && production_rules.size() == 0) {
                // As a special case, the grammar is valid if all 4 components are the empty set. It produces nothing (distinct from the empty langauge).
                return true;
            }

            reason->addReason("No start symbol");
            return false;
        }

        bool startSymbolInSetOfNonTerminals = false;
        int nonTerminalNumber = 1;
        for (auto nt : non_terminals) {
            if (nt == nullptr) {
                std::stringstream ss;
                ss << "Non-terminal " << nonTerminalNumber << " is a null pointer";
                reason->addReason(ss.str());
                return false;
            }

            if (nt->equals(start_symbol)) {
                startSymbolInSetOfNonTerminals = true;
                break;
            }

            ++nonTerminalNumber;
        }

        if (!startSymbolInSetOfNonTerminals) {
            reason->addReason("The start symbol is missing from the set of non-terminals");
            return false;
        }

        int productionRuleNumber = 1;
        for (auto pr : production_rules) {
            if (!pr->isValid()) {
                std::stringstream ss;
                ss << "Production rule " << productionRuleNumber << " is not valid";
                return false;
            }
        }

        return true;
    }
};

}}

#endif // RULE_HPP_INCLUDED
