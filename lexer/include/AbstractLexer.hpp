/*
 *
 * This is a abstract lexer for the toy language. I have done this mainly to
 * have an interface to program the parser against, while making changes to the
 * lexer
 *
 */

#pragma once

#include <string>

namespace toy::lexer {

class AbstractLexer {
public:
  // <filename must be provided
  explicit AbstractLexer(std::string filename) {}
};
}; // namespace toy::lexer
