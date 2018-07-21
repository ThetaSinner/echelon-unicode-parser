#include <iostream>
#include <string>
#include <iostream>
#include <vector>

#include "grammar-parser.hpp"
#include "terminal-symbol.hpp"
#include "non-terminal-symbol.hpp"
#include "production-rule.hpp"
#include "grammar.hpp"

#include "chomsky-test.hpp"

#include "reason.hpp"

#include "cleaner.hpp"

#include "input-sequence.hpp"
#include "string-input-sequence.hpp"
#include "sequence-partition.hpp"
#include "sequence-partition-generator.hpp"

void printIndices(std::vector<unsigned> indices) {
    for (unsigned i = 0; i < indices.size(); i++) {
        std::cout << indices[i];
        if (i + 1 != indices.size()) {
            std::cout << " | ";
        }
    }

    std::cout << "\n";
}

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

    using namespace echelon::parsing;
    api::InputSequence<char>* seq = new api_default_impl::StringInputSequence("pqr");

    auto* gen = new impl::unger::internal::SequencePartitionGenerator<char>(seq, 3);
    while (gen->currentValue() != nullptr) {
        auto indices = gen->currentValue()->getPartitionIndices();
        printIndices(indices);

        gen->moveNext();
    }

    std::cout << "bye" << std::endl;
    return 0;
}
