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
      std::unique_ptr<VarDeclExpr> parseDeclaration();
      std::unique_ptr<ReturnExpr> parseReturn();
      std::unique_ptr<Expr> parseExpression();
      std::unique_ptr<VarType> parseType();
      std::unique_ptr<Expr> parsePrimary();
      std::unique_ptr<Expr> parseBinOpRHS(int aExprPrec, std::unique_ptr<Expr> lhs);
      std::unique_ptr<Expr> parseIdentifierExpr();
      std::unique_ptr<Expr> parseNumberExpr();
      std::unique_ptr<Expr> parseParenExpr();
      std::unique_ptr<Expr> parseTensorLiteralExpr();
      int getTokPrecedence();
      
      template <typename R, typename T, typename U = const char *>
      std::unique_ptr<R> parseError(T &&expected, U &&context = "");

      std::unique_ptr<lexer::AbstractLexer> fLexer;
  };

}