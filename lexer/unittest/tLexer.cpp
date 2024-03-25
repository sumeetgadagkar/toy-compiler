#include "lexer/include/Lexer.hpp"
#include <gtest/gtest.h>

using namespace toy::lexer;

namespace {
  // uitlity function to concert Token to corresponding string
  std::string tokToString(Token& tok) {
    switch (tok) {
      case Token::tok_semicolon : return "tok_semicolon"; 
      case Token::tok_paran_open : return "tok_paran_open";
      case Token::tok_paren_close : return "tok_paren_close";
      case Token::tok_bracket_open : return "tok_bracket_open";
      case Token::tok_bracket_clone : return "tok_bracket_clone";
      case Token::tok_sbracket_open : return "tok_sbracket_open";
      case Token::tok_sbracket_close : return "tok_sbracket_close";
      case Token::tok_eof : return "tok_eof";
      case Token::tok_return : return "tok_return";
      case Token::tok_var : return "tok_var";
      case Token::tok_def : return "tok_def";
      case Token::tok_identifier : return "tok_identifier";
      case Token::tok_number : return "tok_number";
      case Token::tok_plus : return "tok_plus";
      case Token::tok_minus : return "tok_minus";
      case Token::tok_mul : return "tok_mul";
      case Token::tok_sof : return "tok_sof";
      default: return "";
    }
  }
}

TEST(Lexer, Test1) {

  std::stringstream code(R"(
    def main() {
      var a = [1, 2, 3];
      print(a);
    }
  )");

  Lexer lex(std::move(code));

  Token tok;

  while ((tok = lex.getNextToken()) != Token::tok_eof) {
    std::cout << tokToString(tok) << std::endl;
  }
}