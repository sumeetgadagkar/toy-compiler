#include "lexer/include/Lexer.hpp"
#include <cctype>
#include <cstdio>
#include <fstream>
#include <iostream>


namespace toy::lexer {

Lexer::Lexer(const std::string &aFileName) : fCurrToken(Token::tok_sof) {
  fFileName = std::make_shared<std::string>(std::move(aFileName));
  fCurrLocation.file = fFileName;
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
  fCurrLocation.file = fFileName;
  fStream << aStrStream.rdbuf();
  getNextLine();
}

// return the current token in the stream
Token Lexer::getCurrentToken() { return fCurrToken; }

// move to the next token in the stream and return it
Token Lexer::getNextToken() { return fCurrToken = getToken(); }

// return the literal for the current token
std::string Lexer::getLiteral() {
  std::string literal = fCurrLiteral;
  fCurrLiteral = "";
  return literal;
}

// return the start location of the current token
Location Lexer::getCurrentLocation() {
  auto loc = Location();
  return loc;
}

Token Lexer::getToken() {
  // skip whitespace and end of lines
  while (isspace(fCurrChar) || ((fCurrChar == EOF) && !fStream.eof())) {
    fCurrChar = getNextChar();
  }

  // get the location of the current token start
  fCurrLocation.line = fCurrLine;
  fCurrLocation.col = fCurrCol;

  // check for identifier [a-zA-Z][a-zA-Z0-9_]*
  if (std::isalpha(fCurrChar)) {
    fCurrLiteral += (char)fCurrChar;

    while (std::isalnum(fCurrChar = getNextChar()) || fCurrChar == '_') {
      fCurrLiteral += (char)fCurrChar;
    }

    if (fCurrLiteral == "return") {
      // reset literal
      fCurrLiteral = "";
      return Token::tok_return;
    }

    if (fCurrLiteral == "def") {
      // reset literal
      fCurrLiteral = "";
      return Token::tok_def;
    }

    if (fCurrLiteral == "var") {
      // reset literal
      fCurrLiteral = "";
      return Token::tok_var;
    }

    if (fCurrLiteral == "print") {
      // reset literal
      fCurrLiteral = "";
      return Token::tok_print;
    }

    if (fCurrLiteral == "transpose") {
      // reset literal
      fCurrLiteral = "";
      return Token::tok_transpose;
    }

    return Token::tok_identifier;
  }

  // check for number [0-9.]+
  if (std::isdigit(fCurrChar) || fCurrChar == '.') {
    fCurrLiteral += (char)fCurrChar;

    while (std::isdigit(fCurrChar = getNextChar()) || fCurrChar == '.') {
      fCurrLiteral += (char)fCurrChar;
    }

    return Token::tok_number;
  }

  // check for comment #
  if (fCurrChar == '#') {
    // comment lasts until end of line
    auto currLine = fCurrLine;
    while (((fCurrChar = getNextChar()) != EOF) && !fStream.eof() && !fLineStream.eof() && (currLine == fCurrLine)) {
      // do nothing
    }

    // do over
    // if (fCurrChar != EOF) {
      return getToken();
    // }
  }

  // check for EOF
  if (fCurrChar == EOF) {
    return Token::tok_eof;
  }

  // otherwite just return the char
  Token ch = Token(fCurrChar);

  // update for next call
  fCurrChar = getNextChar();

  // reset literal
  fCurrLiteral = "";

  return ch;
}

void Lexer::getNextLine() {
  if (!fStream.eof()) {
    ++fCurrLine;
    std::string line;
    std::getline(fStream, line);
    fLineStream.clear();
    fLineStream.str("");
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
