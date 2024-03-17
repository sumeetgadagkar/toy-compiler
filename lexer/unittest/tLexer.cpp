#include "lexer/include/Lexer.hpp"
#include <gtest/gtest.h>

#include <vector>

namespace {

}

TEST(Lexer, Test1) {

  std::stringstream code(R"(
                         def main() {
                          var a = [1, 2, 3];
                          print(a);
                         }
                         )");

  toy::lexer::Lexer lex(std::move(code));
  std::cout << lex.getNextToken() << " : " << lex.getLiteral() << std::endl;
  
}
