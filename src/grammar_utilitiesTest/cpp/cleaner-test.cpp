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
    auto S = std::make_shared<NonTerminalSymbol<std::string>>();
    S->setValue("S");

    auto A = std::make_shared<NonTerminalSymbol<std::string>>();
    A->setValue("A");

    auto B = std::make_shared<NonTerminalSymbol<std::string>>();
    B->setValue("B");

    auto C = std::make_shared<NonTerminalSymbol<std::string>>();
    C->setValue("C");

    auto D = std::make_shared<NonTerminalSymbol<std::string>>();
    D->setValue("D");

    auto E = std::make_shared<NonTerminalSymbol<std::string>>();
    E->setValue("E");

    auto F = std::make_shared<NonTerminalSymbol<std::string>>();
    F->setValue("F");

    auto a = std::make_shared<TerminalSymbol<char>>();
    a->setValue('a');

    auto b = std::make_shared<TerminalSymbol<char>>();
    b->setValue('b');

    auto c = std::make_shared<TerminalSymbol<char>>();
    c->setValue('c');

    auto d = std::make_shared<TerminalSymbol<char>>();
    d->setValue('d');

    auto e = std::make_shared<TerminalSymbol<char>>();
    e->setValue('e');

    auto f = std::make_shared<TerminalSymbol<char>>();
    f->setValue('f');

    auto rule1 = std::make_shared<ProductionRule>();
    rule1->setKey({S});
    rule1->setValue({A, B});

    auto rule2 = std::make_shared<ProductionRule>();
    rule2->setKey({S});
    rule2->setValue({D, E});

    auto rule3 = std::make_shared<ProductionRule>();
    rule3->setKey({A});
    rule3->setValue({a});

    auto rule4 = std::make_shared<ProductionRule>();
    rule4->setKey({B});
    rule4->setValue({b, C});

    auto rule5 = std::make_shared<ProductionRule>();
    rule5->setKey({C});
    rule5->setValue({c});

    auto rule6 = std::make_shared<ProductionRule>();
    rule6->setKey({D});
    rule6->setValue({d, F});

    auto rule7 = std::make_shared<ProductionRule>();
    rule7->setKey({E});
    rule7->setValue({e});

    auto rule8 = std::make_shared<ProductionRule>();
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
    ASSERT_EQ(S->getValue(), std::static_pointer_cast<NonTerminalSymbol<std::string>>(cleanRule1->getFirstKeySymbol())->getValue());
    ASSERT_EQ((unsigned) 2, cleanRule1->valueLength());

    int cleanRule1ValueSymbolIndex = 0;
    cleanRule1->eachValueSymbol([&cleanRule1ValueSymbolIndex](std::shared_ptr<Symbol> symbol) {
        switch (cleanRule1ValueSymbolIndex) {
            case 0:
                ASSERT_EQ("A", std::static_pointer_cast<NonTerminalSymbol<std::string>>(symbol)->getValue());
                break;
            case 1:
                ASSERT_EQ("B", std::static_pointer_cast<NonTerminalSymbol<std::string>>(symbol)->getValue());
        }
        ++cleanRule1ValueSymbolIndex;
    });

    auto cleanRule2 = grammarForCleaning->getProductionRule(1);
    ASSERT_EQ(A->getValue(), std::static_pointer_cast<NonTerminalSymbol<std::string>>(cleanRule2->getFirstKeySymbol())->getValue());
    ASSERT_EQ((unsigned) 1, cleanRule2->valueLength());
    ASSERT_EQ(a->getValue(), std::static_pointer_cast<TerminalSymbol<char>>(cleanRule2->getLastValueSymbol())->getValue());

    auto cleanRule3 = grammarForCleaning->getProductionRule(2);
    ASSERT_EQ(B->getValue(), std::static_pointer_cast<NonTerminalSymbol<std::string>>(cleanRule3->getFirstKeySymbol())->getValue());
    ASSERT_EQ((unsigned) 2, cleanRule3->valueLength());

    int cleanRule3ValueSymbolIndex = 0;
    cleanRule3->eachValueSymbol([&cleanRule3ValueSymbolIndex](std::shared_ptr<Symbol> symbol) {
        switch (cleanRule3ValueSymbolIndex) {
            case 0:
                ASSERT_EQ('b', std::static_pointer_cast<TerminalSymbol<char>>(symbol)->getValue());
                break;
            case 1:
                ASSERT_EQ("C", std::static_pointer_cast<NonTerminalSymbol<std::string>>(symbol)->getValue());
        }
        ++cleanRule3ValueSymbolIndex;
    });

    auto cleanRule4 = grammarForCleaning->getProductionRule(3);
    ASSERT_EQ(C->getValue(), std::static_pointer_cast<NonTerminalSymbol<std::string>>(cleanRule4->getFirstKeySymbol())->getValue());
    ASSERT_EQ((unsigned) 1, cleanRule4->valueLength());
    ASSERT_EQ(c->getValue(), std::static_pointer_cast<TerminalSymbol<char>>(cleanRule4->getLastValueSymbol())->getValue());
}
