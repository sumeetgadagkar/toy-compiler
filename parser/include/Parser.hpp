/**
 * 
 */

#pragma once
#include <memory>

#include "lexer/include/AbstractLexer.hpp"
#include "AST.hpp"

namespace toy::parser {

  class Parser {
    public:
      Parser(std::unique_ptr<lexer::AbstractLexer> aLexer);
      std::unique_ptr<Module> parseModule();

    private:
      std::unique_ptr<Function> parseDefinition();
      std::unique_ptr<Prototype> parsePrototype();
      std::unique_ptr<ExprList> parseBlock();
      
      template <typename R, typename T, typename U = const char *>
      std::unique_ptr<R> parseError(T &&expected, U &&context = "");

      std::unique_ptr<lexer::AbstractLexer> fLexer;
  };

}