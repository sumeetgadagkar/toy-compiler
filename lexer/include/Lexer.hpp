/*
 *
 * A simple handcoded lexer for the toy language described in
 * https://mlir.llvm.org/docs/Tutorials/Toy/Ch-1/
 *
 * */

#pragma once

#include "lexer/include/AbstractLexer.hpp"
#include <sstream>

namespace toy::lexer {

class Lexer : public AbstractLexer {

public:
  // provide source code file name
  Lexer(const std::string &aFileName);

  // provide a string stream that contains source code
  // the steam is passed by value as we need out own copy
  // the assumption here is that the code is not large
  Lexer(std::stringstream aStrStream);

  // return the current token in the stream
  Token getCurrentToken() override;

  // move to the next token in the stream and return it
  Token getNextToken() override;

  // return the literal for the current token
  std::string getLiteral() override;

  // return the start location of the current token
  Location getCurrentLocation() override;

  ~Lexer() override;

private:
  // get the next token
  Token getToken();
  // get the next line
  void getNextLine();
  // get next char from the buffer
  int getNextChar();

  // source code file name
  std::shared_ptr<std::string> fFileName;
  // the string stream that contains the code
  std::stringstream fStream;
  // current line in the stream
  std::stringstream fLineStream;
  // the current token
  Token fCurrToken;
  // current line
  int fCurrLine = 0;
  // current col
  int fCurrCol = 0;
  // current character, initialized to whitespace
  int fCurrChar = ' ';
  // current token location
  Location fCurrLocation;
  // current literal string
  std::string fCurrLiteral = "";
};
}; // namespace toy::lexer
