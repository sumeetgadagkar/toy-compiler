/*
 *
 * A simple handcoded lexer for the toy language described in
 * https://mlir.llvm.org/docs/Tutorials/Toy/Ch-1/
 *
 * */

#pragma once

#include "lexer/include/AbstractLexer.hpp"

namespace toy::lexer {

class Lexer : public AbstractLexer {

public:
  // provide source code file name
  Lexer(const std::string &aFileName);

  // return the current token in the stream
  Token getCurrentToken() override;

  // move to the next token in the stream and return it
  Token getNextToken() override;

  // return the literal for the current token
  std::string getLiteral() override;

  // return the start location of the current token
  Location getCurrentLocation() override;

  ~Lexer() override;
};
}; // namespace toy::lexer
