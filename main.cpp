#include "lexer/include/Lexer.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
  std::stringstream code("return");
  toy::lexer::Lexer lex(std::move(code));
  std::cout << lex.getNextToken() << " : " << lex.getLiteral() << std::endl;
  return 0;
}
