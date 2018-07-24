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

    auto *S = new NonTerminalSymbol<std::string>();
    S->setValue("S");

    auto *A = new NonTerminalSymbol<std::string>();
    A->setValue("A");

    auto *B = new NonTerminalSymbol<std::string>();
    B->setValue("B");

    auto *C = new NonTerminalSymbol<std::string>();
    C->setValue("C");

    auto *D = new NonTerminalSymbol<std::string>();
    D->setValue("D");

    auto *E = new NonTerminalSymbol<std::string>();
    E->setValue("E");

    auto *F = new NonTerminalSymbol<std::string>();
    F->setValue("F");

    auto *a = new TerminalSymbol<char>();
    a->setValue('a');

    auto *b = new TerminalSymbol<char>();
    b->setValue('b');

    auto *c = new TerminalSymbol<char>();
    c->setValue('c');

    auto *d = new TerminalSymbol<char>();
    d->setValue('d');

    auto *e = new TerminalSymbol<char>();
    e->setValue('e');

    auto *f = new TerminalSymbol<char>();
    f->setValue('f');

    ProductionRule *rule1 = new ProductionRule();
    rule1->setKey({S});
    rule1->setValue({A, B});

    ProductionRule *rule2 = new ProductionRule();
    rule2->setKey({S});
    rule2->setValue({D, E});

    ProductionRule *rule3 = new ProductionRule();
    rule3->setKey({A});
    rule3->setValue({a});

    ProductionRule *rule4 = new ProductionRule();
    rule4->setKey({B});
    rule4->setValue({b, C});

    ProductionRule *rule5 = new ProductionRule();
    rule5->setKey({C});
    rule5->setValue({c});

    ProductionRule *rule6 = new ProductionRule();
    rule6->setKey({D});
    rule6->setValue({d, F});

    ProductionRule *rule7 = new ProductionRule();
    rule7->setKey({E});
    rule7->setValue({e});

    ProductionRule *rule8 = new ProductionRule();
    rule8->setKey({F});
    rule8->setValue({f, D});

    auto *grammarForCleaning = new Grammar();
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

    GrammarCleaner cleaner;
    auto *cleaningReason = new Reason();
    ASSERT_TRUE(cleaner.clean(grammarForCleaning, cleaningReason));

    // Clean grammar
    /*
        S -> AB
        A -> a
        B -> bC
        C -> c
    */
    ASSERT_EQ((unsigned) 4, grammarForCleaning->numberOfProductionRules());

    auto cleanRule1 = grammarForCleaning->getProductionRule(0);
    ASSERT_EQ(S->getValue(), ((NonTerminalSymbol<std::string>*) cleanRule1->getFirstKeySymbol())->getValue());
    ASSERT_EQ((unsigned) 2, cleanRule1->valueLength());

    int cleanRule1ValueSymbolIndex = 0;
    cleanRule1->eachValueSymbol([&cleanRule1ValueSymbolIndex](Symbol* symbol) {
        switch (cleanRule1ValueSymbolIndex) {
            case 0:
                ASSERT_EQ("A", ((NonTerminalSymbol<std::string>*) symbol)->getValue());
                break;
            case 1:
                ASSERT_EQ("B", ((NonTerminalSymbol<std::string>*) symbol)->getValue());
        }
        ++cleanRule1ValueSymbolIndex;
    });

    auto cleanRule2 = grammarForCleaning->getProductionRule(1);
    ASSERT_EQ(A->getValue(), ((NonTerminalSymbol<std::string>*) cleanRule2->getFirstKeySymbol())->getValue());
    ASSERT_EQ((unsigned) 1, cleanRule2->valueLength());
    ASSERT_EQ(a->getValue(), ((TerminalSymbol<char>*) cleanRule2->getLastValueSymbol())->getValue());

    auto cleanRule3 = grammarForCleaning->getProductionRule(2);
    ASSERT_EQ(B->getValue(), ((TerminalSymbol<std::string>*) cleanRule3->getFirstKeySymbol())->getValue());
    ASSERT_EQ((unsigned) 2, cleanRule3->valueLength());

    int cleanRule3ValueSymbolIndex = 0;
    cleanRule3->eachValueSymbol([&cleanRule3ValueSymbolIndex](Symbol* symbol) {
        switch (cleanRule3ValueSymbolIndex) {
            case 0:
                ASSERT_EQ('b', ((TerminalSymbol<char>*) symbol)->getValue());
                break;
            case 1:
                ASSERT_EQ("C", ((NonTerminalSymbol<std::string>*) symbol)->getValue());
        }
        ++cleanRule3ValueSymbolIndex;
    });

    auto cleanRule4 = grammarForCleaning->getProductionRule(3);
    ASSERT_EQ(C->getValue(), ((NonTerminalSymbol<std::string>*) cleanRule4->getFirstKeySymbol())->getValue());
    ASSERT_EQ((unsigned) 1, cleanRule4->valueLength());
    ASSERT_EQ(c->getValue(), ((TerminalSymbol<char>*) cleanRule4->getLastValueSymbol())->getValue());
}
