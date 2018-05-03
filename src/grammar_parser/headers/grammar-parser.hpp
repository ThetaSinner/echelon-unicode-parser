#ifndef GRAMMAR_PARSER_HPP_INCLUDED
#define GRAMMAR_PARSER_HPP_INCLUDED

#include "antlr4-runtime.h"
#include "EchelonParser.h"
#include "EchelonBaseListener.h"

class EchelonCustomListener : public EchelonBaseListener {
public:
  void enterNumber(EchelonParser::NumberContext *ctx) override;
};

#endif
