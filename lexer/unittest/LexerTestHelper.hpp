#pragma once

#include "lexer/include/Lexer.hpp"

#include <iostream>
#include <sstream>
#include <vector>

using namespace toy::lexer;

// uitlity function to get the underlying string from a stream
__attribute__((used)) inline std::string
getStringFromStream(std::stringstream &ss) {
  return ss.str();
}

// uitlity function to concert Token to corresponding string
inline std::string tokToString(const Token &tok) {
  switch (tok) {
  case Token::tok_semicolon:
    return "tok_semicolon";
  case Token::tok_paren_open:
    return "tok_paren_open";
  case Token::tok_paren_close:
    return "tok_paren_close";
  case Token::tok_bracket_open:
    return "tok_bracket_open";
  case Token::tok_bracket_close:
    return "tok_bracket_close";
  case Token::tok_sbracket_open:
    return "tok_sbracket_open";
  case Token::tok_sbracket_close:
    return "tok_sbracket_close";
  case Token::tok_eof:
    return "tok_eof";
  case Token::tok_return:
    return "tok_return";
  case Token::tok_var:
    return "tok_var";
  case Token::tok_def:
    return "tok_def";
  case Token::tok_identifier:
    return "tok_identifier";
  case Token::tok_number:
    return "tok_number";
  case Token::tok_plus:
    return "tok_plus";
  case Token::tok_minus:
    return "tok_minus";
  case Token::tok_mul:
    return "tok_mul";
  case Token::tok_sof:
    return "tok_sof";
  case Token::tok_equals:
    return "tok_equals";
  case Token::tok_comma:
    return "tok_comma";
  case Token::tok_print:
    return "tok_print";
  default:
    return "";
  }
}

// utility to get tokens from the leer
inline std::vector<TokType> getToksFromLexer(Lexer &aLexer) {
  Token tok;
  std::string literal;

  std::vector<TokType> actual_toks;

  while ((tok = aLexer.getNextToken()) != Token::tok_eof) {
    if (tok == Token::tok_identifier || tok == Token::tok_number) {
      literal = aLexer.getLiteral();
      actual_toks.push_back(TokWithLieral{tok, literal});
    } else {
      actual_toks.push_back(tok);
    }
  }

  return actual_toks;
}

// utility function to check if two std::vector<TokType> are equal
inline bool areToksEqual(const std::vector<TokType> &v1,
                         const std::vector<TokType> &v2) {
  // check size
  if (v1.size() != v2.size()) {
    return false;
  }

  for (size_t i = 0; i < v1.size(); i++) {
    // check if same variant
    if (v1[i].index() != v2[i].index()) {
      return false;
    }

    // check for Token variant
    if (std::holds_alternative<Token>(v1[i])) {
      if (std::get<Token>(v1[i]) != std::get<Token>(v2[i])) {
        return false;
      }
    }

    // check for TokWithLiteral
    if (std::holds_alternative<TokWithLieral>(v1[i])) {
      if ((std::get<TokWithLieral>(v1[i]).tok !=
           std::get<TokWithLieral>(v2[i]).tok) ||
          (std::get<TokWithLieral>(v1[i]).literal !=
           std::get<TokWithLieral>(v2[i]).literal)) {
        return false;
      }
    }
  }

  return true;
}

// utility to print a vector of tokens
__attribute__((used)) inline void
printTokens(const std::vector<TokType> &toks) {
  for (size_t i = 0; i < toks.size(); i++) {
    if (std::holds_alternative<Token>(toks[i])) {
      std::cout << tokToString(std::get<Token>(toks[i])) << std::endl;
    } else {
      auto tok = std::get<TokWithLieral>(toks[i]);
      std::cout << tokToString(tok.tok) << " : " << tok.literal << std::endl;
    }
  }
}