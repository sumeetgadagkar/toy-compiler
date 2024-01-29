#include "lexer/include/Lexer.hpp"

namespace toy::lexer {

Lexer::Lexer(const std::string &aFileName) {}

// return the current token in the stream
Token Lexer::getCurrentToken() { return Token::tok_number; }

// move to the next token in the stream and return it
Token Lexer::getNextToken() { return Token::tok_number; }

// return the literal for the current token
std::string Lexer::getLiteral() { return "Hello"; }

// return the start location of the current token
Location Lexer::getCurrentLocation() {
  auto loc = Location();
  return loc;
}

Lexer::~Lexer() {}

}; // namespace toy::lexer
