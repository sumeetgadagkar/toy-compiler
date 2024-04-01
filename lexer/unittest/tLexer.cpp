#include "lexer/include/Lexer.hpp"
#include "LexerTestHelper.hpp"
#include <gtest/gtest.h>

using namespace toy::lexer;

TEST(Lexer, SimpleMain) {

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

TEST(Lexer, UserFunction) {

  std::stringstream code(R"(
    
    def user_fn(in) {
      return in * 2;
    }

    def main() {
      var a = [1, 2, 3];
      var b = user_fn(a);
    } 
    
  )");

  std::vector<TokType> expected_toks = {
    // user_fn and it's contents
    Token::tok_def,
    TokWithLieral{Token::tok_identifier, "user_fn"},
    Token::tok_paren_open,
    TokWithLieral{Token::tok_identifier, "in"},
    Token::tok_paren_close,
    Token::tok_bracket_open,
    Token::tok_return,
    TokWithLieral{Token::tok_identifier, "in"},
    Token::tok_mul,
    TokWithLieral{Token::tok_number, "2"},
    Token::tok_semicolon,
    Token::tok_bracket_close,
    // main and it's contents
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
    Token::tok_var,
    TokWithLieral{Token::tok_identifier, "b"},
    Token::tok_equals,
    TokWithLieral{Token::tok_identifier, "user_fn"},
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

TEST(Lexer, Comment) {

  std::stringstream code(R"(
    
    def main() {
      var a = [1, 2, 3];
      # This is a comment
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