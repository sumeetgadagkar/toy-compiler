#include "lexer/include/Lexer.hpp"
#include "LexerTestHelper.hpp"
#include <gtest/gtest.h>

using namespace toy::lexer;

TEST(Lexer, Test1) {

  std::stringstream code(R"(
    
    def main() {
      var a = [1, 2, 3];
      print(a);
    } 
    
  )");

  std::vector<TokType> expected_toks = {
    Token::tok_def,
    TokWithLieral{Token::tok_identifier, "main"},
    Token::tok_paren_open,
    Token::tok_paren_close,
    Token::tok_bracket_open,
    Token::tok_var,
    TokWithLieral{Token::tok_identifier, "a"},
    Token::tok_equals,
    Token::tok_sbracket_open,
    TokWithLieral{Token::tok_number, "1"},
    Token::tok_comma,
    TokWithLieral{Token::tok_number, "2"},
    Token::tok_comma,
    TokWithLieral{Token::tok_number, "3"},
    Token::tok_sbracket_close,
    Token::tok_semicolon,
    Token::tok_print,
    Token::tok_paren_open,
    TokWithLieral{Token::tok_identifier, "a"},
    Token::tok_paren_close,
    Token::tok_semicolon,
    Token::tok_bracket_close
  };

  // prime the lexer
  Lexer lex(std::move(code));

  // get actual tokens from the lexer
  std::vector<TokType> actual_toks = getToksFromLexer(lex);

  // check if equal
  EXPECT_TRUE(areToksEqual(actual_toks, expected_toks));
}