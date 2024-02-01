/*
 *
 * This is a abstract lexer for the toy language. I have done this mainly to
 * have an interface to program the parser against, while making changes to the
 * lexer
 *
 */

#pragma once

#include <memory>
#include <string>

namespace toy::lexer {

// structure to hold the location of a token in a file
struct Location {
  std::shared_ptr<std::string> file;
  int line;
  int col;
};

// list of tokens that the lexer can return
enum Token : int {
  tok_semicolon = ';',
  tok_paran_open = '(',
  tok_paren_close = ')',
  tok_bracket_open = '{',
  tok_bracket_clone = '}',
  tok_sbracket_open = '[',
  tok_sbracket_close = ']',

  tok_eof = -1,

  tok_return = -2,
  tok_var = -3,
  tok_def = -4,

  tok_identifier = -5,
  tok_number = -6,

  tok_plus = -7,
  tok_minus = -8,
  tok_mul = -9,

  tok_sof = -10,
};

class AbstractLexer {
public:
  virtual ~AbstractLexer() = 0;

  // return the current token in the stream
  virtual Token getCurrentToken() = 0;

  // move to the next token in the stream and return it
  virtual Token getNextToken() = 0;

  // return the literal for the current token
  virtual std::string getLiteral() = 0;

  // return the start location of the current token
  virtual Location getCurrentLocation() = 0;
};

inline AbstractLexer::~AbstractLexer() = default;

}; // namespace toy::lexer
