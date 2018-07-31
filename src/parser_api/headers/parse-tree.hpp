#ifndef PARSE_TREE_HPP_INCLUDED
#define PARSE_TREE_HPP_INCLUDED

#include <memory>
#include <vector>
#include "symbol.hpp"

namespace echelon { namespace parsing { namespace api {

class ParseTree {
public:
    static std::shared_ptr<ParseTree> createWithSymbol(std::shared_ptr<Symbol> symbol) {
        auto root_node = std::make_shared<ParseTree>();
        root_node->symbol = symbol;
        return root_node;
    }

    void addChild(std::shared_ptr<Symbol> symbol) {
        auto child_node = createWithSymbol(symbol);
        children.push_back(child_node);
    }

    void addSubTree(std::shared_ptr<ParseTree> subTree) {
        children.push_back(subTree);
    }

	std::shared_ptr<Symbol> getCurrentSymbol() {
		return symbol;
	}

	std::vector<std::shared_ptr<ParseTree>> getChildren() {
		return children;
	}

private:
    std::shared_ptr<Symbol> symbol;
    std::vector<std::shared_ptr<ParseTree>> children;
};

}}}

#endif // PARSE_TREE_HPP_INCLUDED
