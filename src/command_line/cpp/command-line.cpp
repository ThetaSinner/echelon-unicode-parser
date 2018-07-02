#include <iostream>
#include <string>
#include <iostream>

#include "grammar-parser.hpp"
#include "terminal-symbol.hpp"
#include "non-terminal-symbol.hpp"
#include "production-rule.hpp"
#include "grammar.hpp"

#include "chomsky-test.hpp"

#include "reason.hpp"

#include "cleaner.hpp"

void testGrammarClean();

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Please provide an input file as argument 1" << std::endl;
        return 0;
    }
    
    // std::ifstream stream;
    // stream.open(argv[1]);
    
    GrammarParser grammarParser;
    grammarParser.test();

    auto *symbol_T = new NonTerminalSymbol<std::string>();
    symbol_T->setValue("T");
    
    if (symbol_T->getType() == SymbolType::NonTerminal) {
        std::cout << "symbol_T is a non-terminal\n";
    }
    else {
        std::cout << "symbol_T is a terminal\n";
    }

    auto *symbol_b = new TerminalSymbol<std::string>();
    symbol_b->setValue("b");

    if (symbol_b->getType() == SymbolType::NonTerminal) {
        std::cout << "symbol_b is a non-terminal\n";
    }
    else {
        std::cout << "symbol_b is a terminal\n";
    }

    auto r = new ProductionRule<std::string>();
    r->setKey({symbol_T});
    r->setValue({symbol_b});

    r->print();

    auto badRule = new ProductionRule<std::string>();
    badRule->setKey({symbol_b});
    badRule->setValue({symbol_T});

    badRule->print();

    std::cout << "\nYour grammar is: \n";
    using namespace echelon::grammar_model;
    Grammar<std::string> g;
    g.addNonTerminal(symbol_T);
    g.setStartSymbol(symbol_T);
    g.addTerminal(symbol_b);
    g.addProductionRule(r);

    g.print();

    using namespace echelon::grammar_utilities;
    if (ChomskyTest<std::string>::isTypeThree(&g)) {
        std::cout << "g is a type 3 grammar\n";
    }
    else {
        std::cout << "g is NOT a type 3 grammar\n";
    }

    auto *reason = new echelon::Reason();
    reason->addReason("low level reason");
    reason->addReason("higher level reason");
    reason->replay();

    testGrammarClean();

    std::cout << "bye" << std::endl;
    return 0;
}

void testGrammarClean()
{
    // Dirty Grammar
    /*
        Ss ---> AB|DE
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


    using namespace echelon::grammar_model;
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

    using namespace echelon;
    auto *reason = new Reason();

    if (grammarForCleaning->isValid(reason)) {
        std::cout << "Your grammar for cleaning is valid\n";
    }
    else {
        std::cout << "Your grammar for cleaning is NOT valid\n";

        reason->replay();
    }

    grammarForCleaning->print();

    using namespace echelon::grammar_utilities::cleaning;
    GrammarCleaner<std::string> cleaner;
    auto *cleaningReason = new Reason();
    if (!cleaner.clean(grammarForCleaning, cleaningReason)) {
        std::cout << "Failed to clean the grammar\n";
    }

    grammarForCleaning->print();
}
