#ifndef GRAMMAR_HPP_INCLUDED
#define GRAMMAR_HPP_INCLUDED

#include <vector>

#include "symbol.hpp"
#include "production-rule.hpp"

template<typename T>
class Grammar {
    std::vector<Symbol<T>*> terminals;
    std::vector<Symbol<T>*> non_terminals;
    Symbol<T>* start_symbol;
    std::vector<ProductionRule<T>*> production_rules;

public:
    void addTerminal(Symbol<T>* terminal) {
        terminals.push_back(terminal);
    }

    void addNonTerminal(Symbol<T>* non_terminal) {
        non_terminals.push_back(non_terminal);
    }

    void setStartSymbol(Symbol<T>* start_symbol) {
        this->start_symbol = start_symbol;
    }

    void addProductionRule(ProductionRule<T>* production_rule) {
        production_rules.push_back(production_rule);
    }

    void print() {
        std::cout << "The start symbol is " << start_symbol->getValue() << "\n";

        if (production_rules.size() == 0) {
            std::cout << "No rules\n";
        }
        else if (production_rules.size() == 1) {
            std::cout << "There is 1 rule. It is\n";
        }
        else {
            std::cout << "There are " << production_rules.size() << " rules. They are\n";
        }
        
        for (auto pr : production_rules) {
            pr->print();
        }

        std::cout << std::endl;
    }
};

#endif // RULE_HPP_INCLUDED
