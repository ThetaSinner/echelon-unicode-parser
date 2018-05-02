#ifndef GRAMMAR_PARSER_HPP_INCLUDED
#define GRAMMAR_PARSER_HPP_INCLUDED

#include "antlr4-runtime.h"
#include "EchelonBaseListener.h"

using namespace org::antlr::v4::runtime;

class EchelonCustomListener : public EchelonBaseListener {
public:
  void enterNumber(ParserRuleContext *ctx) override;
};

#endif
