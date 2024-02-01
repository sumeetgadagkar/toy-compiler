#include "lexer/include/Lexer.hpp"
#include <cctype>
#include <cstdio>
#include <fstream>
#include <iostream>

namespace toy::lexer {

Lexer::Lexer(const std::string &aFileName) : fCurrToken(Token::tok_sof) {
  fFileName = std::make_shared<std::string>(std::move(aFileName));
  // read file contents
  std::ifstream code_file(aFileName);
  if (!code_file.is_open()) {
    std::cerr << "Invalid input file : " << aFileName << "." << std::endl;
  }
  // store contents of file into a stream
  fStream << code_file.rdbuf();
  getNextLine();
}

Lexer::Lexer(std::stringstream aStrStream) : fCurrToken(Token::tok_sof) {
  fFileName = std::make_shared<std::string>("buffer");
  fStream << aStrStream.rdbuf();
  getNextLine();
}

// return the current token in the stream
Token Lexer::getCurrentToken() { return fCurrToken; }

// move to the next token in the stream and return it
Token Lexer::getNextToken() { return Token::tok_number; }

// return the literal for the current token
std::string Lexer::getLiteral() { return "Hello"; }

// return the start location of the current token
Location Lexer::getCurrentLocation() {
  auto loc = Location();
  return loc;
}

void Lexer::getNextLine() {
  if (!fStream.str().empty()) {
    ++fCurrLine;
    std::string line;
    std::getline(fStream, line);
    fLineStream << line;
  } else {
    fLineStream << "";
  }
}

int Lexer::getNextChar() {
  // check if stream is empty
  if (fLineStream.eof()) {
    return EOF;
  }
  ++fCurrCol;
  auto nextChar = fLineStream.get();
  if (fLineStream.eof()) {
    getNextLine();
  }

  return nextChar;
}

Lexer::~Lexer() {}

}; // namespace toy::lexer
