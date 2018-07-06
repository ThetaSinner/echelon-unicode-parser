#include <gtest/gtest.h>

#include <string>

#include "grammar.hpp"
#include "non-terminal-symbol.hpp"
#include "terminal-symbol.hpp"
#include "production-rule.hpp"

#include "cleaner.hpp"

using namespace echelon::grammar_model;
using namespace echelon::grammar_utilities::cleaning;
using namespace echelon::diagnostics;

TEST(GrammarCleaner, CleanGrammar) {
    // Dirty Grammar
    /*
        S ---> AB|DE
        A--->a
        B--->bC
        C--->c
        D--->dF
        E--->e
        F--->fD
    */

    Symbol<std::string> *S = new NonTerminalSymbol<std::string>();
    S->setValue("S");

    Symbol<std::string> *A = new NonTerminalSymbol<std::string>();
    A->setValue("A");

    Symbol<std::string> *B = new NonTerminalSymbol<std::string>();
    B->setValue("B");

    Symbol<std::string> *C = new NonTerminalSymbol<std::string>();
    C->setValue("C");

    Symbol<std::string> *D = new NonTerminalSymbol<std::string>();
    D->setValue("D");

    Symbol<std::string> *E = new NonTerminalSymbol<std::string>();
    E->setValue("E");

    Symbol<std::string> *F = new NonTerminalSymbol<std::string>();
    F->setValue("F");

    Symbol<std::string> *a = new TerminalSymbol<std::string>();
    a->setValue("a");

    Symbol<std::string> *b = new TerminalSymbol<std::string>();
    b->setValue("b");

    Symbol<std::string> *c = new TerminalSymbol<std::string>();
    c->setValue("c");

    Symbol<std::string> *d = new TerminalSymbol<std::string>();
    d->setValue("d");

    Symbol<std::string> *e = new TerminalSymbol<std::string>();
    e->setValue("e");

    Symbol<std::string> *f = new TerminalSymbol<std::string>();
    f->setValue("f");

    ProductionRule<std::string> *rule1 = new ProductionRule<std::string>();
    rule1->setKey({S});
    rule1->setValue({A, B});

    ProductionRule<std::string> *rule2 = new ProductionRule<std::string>();
    rule2->setKey({S});
    rule2->setValue({D, E});

    ProductionRule<std::string> *rule3 = new ProductionRule<std::string>();
    rule3->setKey({A});
    rule3->setValue({a});

    ProductionRule<std::string> *rule4 = new ProductionRule<std::string>();
    rule4->setKey({B});
    rule4->setValue({b, C});

    ProductionRule<std::string> *rule5 = new ProductionRule<std::string>();
    rule5->setKey({C});
    rule5->setValue({c});

    ProductionRule<std::string> *rule6 = new ProductionRule<std::string>();
    rule6->setKey({D});
    rule6->setValue({d, F});

    ProductionRule<std::string> *rule7 = new ProductionRule<std::string>();
    rule7->setKey({E});
    rule7->setValue({e});

    ProductionRule<std::string> *rule8 = new ProductionRule<std::string>();
    rule8->setKey({F});
    rule8->setValue({f, D});

    auto *grammarForCleaning = new Grammar<std::string>();
    grammarForCleaning->setStartSymbol(S);

    grammarForCleaning->addNonTerminal(S);
    grammarForCleaning->addNonTerminal(A);
    grammarForCleaning->addNonTerminal(B);
    grammarForCleaning->addNonTerminal(C);
    grammarForCleaning->addNonTerminal(D);
    grammarForCleaning->addNonTerminal(E);
    grammarForCleaning->addNonTerminal(F);

    grammarForCleaning->addTerminal(a);
    grammarForCleaning->addTerminal(b);
    grammarForCleaning->addTerminal(c);
    grammarForCleaning->addTerminal(d);
    grammarForCleaning->addTerminal(e);
    grammarForCleaning->addTerminal(f);

    grammarForCleaning->addProductionRule(rule1);
    grammarForCleaning->addProductionRule(rule2);
    grammarForCleaning->addProductionRule(rule3);
    grammarForCleaning->addProductionRule(rule4);
    grammarForCleaning->addProductionRule(rule5);
    grammarForCleaning->addProductionRule(rule6);
    grammarForCleaning->addProductionRule(rule7);
    grammarForCleaning->addProductionRule(rule8);

    ASSERT_TRUE(grammarForCleaning->isValid(nullptr));

    GrammarCleaner<std::string> cleaner;
    auto *cleaningReason = new Reason();
    ASSERT_TRUE(cleaner.clean(grammarForCleaning, cleaningReason));

    // Clean grammar
    /*
        S -> AB
        A -> a
        B -> bC
        C -> c
    */
    ASSERT_EQ(4, grammarForCleaning->numberOfProductionRules());

    auto cleanRule1 = grammarForCleaning->getProductionRule(0);
    ASSERT_EQ(S->getValue(), cleanRule1->getFirstKeySymbol()->getValue());
    ASSERT_EQ(2, cleanRule1->valueLength());

    int cleanRule1ValueSymbolIndex = 0;
    cleanRule1->eachValueSymbol([&cleanRule1ValueSymbolIndex](Symbol<std::string>* symbol) {
        switch (cleanRule1ValueSymbolIndex) {
            case 0:
                ASSERT_EQ("A", symbol->getValue());
                break;
            case 1:
                ASSERT_EQ("B", symbol->getValue());
        }
        ++cleanRule1ValueSymbolIndex;
    });

    auto cleanRule2 = grammarForCleaning->getProductionRule(1);
    ASSERT_EQ(A->getValue(), cleanRule2->getFirstKeySymbol()->getValue());
    ASSERT_EQ(1, cleanRule2->valueLength());
    ASSERT_EQ(a->getValue(), cleanRule2->getLastValueSymbol()->getValue());

    auto cleanRule3 = grammarForCleaning->getProductionRule(2);
    ASSERT_EQ(B->getValue(), cleanRule3->getFirstKeySymbol()->getValue());
    ASSERT_EQ(2, cleanRule3->valueLength());

    int cleanRule3ValueSymbolIndex = 0;
    cleanRule3->eachValueSymbol([&cleanRule3ValueSymbolIndex](Symbol<std::string>* symbol) {
        switch (cleanRule3ValueSymbolIndex) {
            case 0:
                ASSERT_EQ("b", symbol->getValue());
                break;
            case 1:
                ASSERT_EQ("C", symbol->getValue());
        }
        ++cleanRule3ValueSymbolIndex;
    });

    auto cleanRule4 = grammarForCleaning->getProductionRule(3);
    ASSERT_EQ(C->getValue(), cleanRule4->getFirstKeySymbol()->getValue());
    ASSERT_EQ(1, cleanRule4->valueLength());
    ASSERT_EQ(c->getValue(), cleanRule4->getLastValueSymbol()->getValue());
}
