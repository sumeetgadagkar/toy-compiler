#include <iostream>
#include <algorithm>

#include "parser/include/Parser.hpp"

namespace toy::parser {
  
  Parser::Parser(std::unique_ptr<lexer::AbstractLexer> aLexer) : fLexer(std::move(aLexer)) {}

  std::unique_ptr<Module> Parser::parseModule() {
    // prime the lexer
    fLexer->getNextToken();

    // parse functions one at a time
    std::vector<std::unique_ptr<Function>> functions;
    while (auto f = parseDefinition()) {
      functions.push_back(std::move(f));
      if (fLexer->getCurrentToken() == lexer::tok_eof) {
        break;
      }
    }

    if (fLexer->getCurrentToken() != lexer::tok_eof) {
      return parseError<Module>("nothing", "at end of module");
    }

    return std::make_unique<Module>(functions);
  }

  // definition ::= prototype block
  std::unique_ptr<Function> Parser::parseDefinition() {
    auto proto = parsePrototype();
    
    if (!proto) {
      return nullptr;
    }

    if (auto block = parseBlock()) {
      return std::make_unique<Function>(std::move(proto), std::move(block));
    }

    return nullptr;
  }

  // prototype ::= def id '(' decl_list ')'
  // decl_list ::= identifier | identifier, decl_list
  std::unique_ptr<Prototype> Parser::parsePrototype() {
    auto fcn_loc = fLexer->getCurrentLocation();

    // if we do not see def at the start, error
    if (fLexer->getCurrentToken() != lexer::tok_def) {
      return parseError<Prototype>("def", "in prototype");
    }   
    fLexer->consume(lexer::tok_def);
    
    // if we do not see an id after the def, error
    if (fLexer->getCurrentToken() != lexer::tok_identifier) {
      return parseError<Prototype>("function name", "in prototype");
    }

    auto fcnName = fLexer->getLiteral();
    fLexer->consume(lexer::tok_identifier);

    if (fLexer->getCurrentToken() != lexer::tok_paren_open) {
      return parseError<Prototype>("(", "in prototype");
    }
    fLexer->consume(lexer::tok_paren_open);

    std::vector<std::unique_ptr<VarExpr>> args;

    // check if arguments exist
    if (fLexer->getCurrentToken() != lexer::tok_paren_close) {
      while(true) {
        std::string varName = fLexer->getLiteral();
        auto loc = fLexer->getCurrentLocation();
        fLexer->consume(lexer::tok_identifier);
        auto arg = std::make_unique<VarExpr>(varName, loc);
        args.push_back(arg);
        
        // check if more args exist
        if (fLexer->getCurrentToken() != lexer::tok_comma) {
          break;
        }
        fLexer->consume(lexer::tok_comma);
        if (fLexer->getCurrentToken() != lexer::tok_identifier) {
          return parseError<Prototype>("identifier", "after ',' in function param list");
        }
      }
    }

    if (fLexer->getCurrentToken() != lexer::tok_paren_close) {
      return parseError<Prototype>(")", "to end function protoype");
    }

    fLexer->consume(lexer::tok_paren_close);
    return std::make_unique<Prototype>(fcnName, std::move(args), std::move(fcn_loc));
  }

  // Parse a block: a list of expression separated by semicolons and wrapped in
  // curly braces.
  //
  // block ::= { expression_list }
  // expression_list ::= block_expr ; expression_list
  // block_expr ::= decl | "return" | expr
  std::unique_ptr<ExprList> Parser::parseBlock() {
    if (fLexer->getCurrentToken() != lexer::tok_bracket_open) {
      return parseError<ExprList>("{", "to begin the block");
    }

    fLexer->consume(lexer::tok_bracket_open);

    auto exprList = std::make_unique<ExprList>();

    // consume semi colons
    while(fLexer->getCurrentToken() == lexer::tok_semicolon) {
      fLexer->consume(lexer::tok_semicolon);
    }

    while (fLexer->getCurrentToken() != lexer::tok_bracket_close && fLexer->getCurrentToken() != lexer::tok_eof) {
      
      if (fLexer->getCurrentToken() == lexer::tok_var) {
        // variable declaration
        auto varDecl = parseDeclaration();
        if (!varDecl) {
          return nullptr;
        }
        exprList->push_back(std::move(varDecl));
      } else if (fLexer->getCurrentToken() == lexer::tok_return) {
        // return statement
        auto ret = parseReturn();
        if (!ret) {
          return nullptr;
        }
        exprList->push_back(std::move(ret));
      } else {
        // general expression
        auto expr = parseExpression();
        if (!expr) {
          return nullptr;
        }
        exprList->push_back(std::move(expr));
      }

      // ensure elements are seperated by semicolon
      if (fLexer->getCurrentToken() != lexer::tok_semicolon) {
        return parseError<ExprList>(";", "after expression");
      }

      // consume semi colons
      while(fLexer->getCurrentToken() == lexer::tok_semicolon) {
        fLexer->consume(lexer::tok_semicolon);
      }
    }

    if (fLexer->getCurrentToken() != lexer::tok_bracket_close) {
      return parseError<ExprList>("}", "to close block");
    }

    fLexer->consume(lexer::tok_bracket_close);
    return exprList;
  }

  std::unique_ptr<VarDeclExpr> Parser::parseDeclaration() {
    if (fLexer->getCurrentToken() != lexer::tok_var) {
      return parseError<VarDeclExpr>("var", "to begin declaration");
    }
    auto loc = fLexer->getCurrentLocation();
    fLexer->consume(lexer::tok_var);

    if (fLexer->getCurrentToken() != lexer::tok_identifier) {
      return parseError<VarDeclExpr>("identifier", "after 'var' declaration");
    }

    std::string name = fLexer->getLiteral();
    fLexer->consume(lexer::tok_identifier);

    std::unique_ptr<VarType> type;
    if (fLexer->getCurrentToken() == lexer::tok_shape_open) {
      type = parseType();
      if (!type) {
        return nullptr;
      }
    }

    if (!type) {
      type = std::make_unique<VarType>();
    }
    fLexer->consume(lexer::tok_equals);
    auto expr = parseExpression();
    
    return std::make_unique<VarDeclExpr>(name, std::move(*type), std::move(expr), std::move(loc));
  }

  std::unique_ptr<ReturnExpr> Parser::parseReturn() {
    auto loc = fLexer->getCurrentLocation();
    fLexer->consume(lexer::tok_return);

    // return takes an optional argument
    std::optional<std::unique_ptr<Expr>> expr;
    if (fLexer->getCurrentToken() != lexer::tok_semicolon) {
      expr = parseExpression();
      if (!expr) {
        return nullptr;
      }
    }
    return std::make_unique<ReturnExpr>(std::move(expr), std::move(loc));
  }

  std::unique_ptr<Expr> Parser::parseExpression() {
    auto lhs = parsePrimary();
    if (!lhs) {
      return nullptr;
    }
    return parseBinOpRHS(0, std::move(lhs));
  }

  std::unique_ptr<VarType> Parser::parseType() {
    if (fLexer->getCurrentToken() != lexer::tok_shape_open) {
      return parseError<VarType>("<", "to begin type");
    }
    fLexer->consume(lexer::tok_shape_open);

    auto type = std::make_unique<VarType>();

    while (fLexer->getCurrentToken() == lexer::tok_number) {
      type->shape.push_back(std::stoi(fLexer->getLiteral()));
      fLexer->consume(lexer::tok_number);
      if (fLexer->getCurrentToken() == lexer::tok_comma) {
        fLexer->consume(lexer::tok_comma);
      }
    }

    if (fLexer->getCurrentToken() != lexer::tok_shape_close) {
      return parseError<VarType>(">", "to end type");
    }
    fLexer->consume(lexer::tok_shape_close);

    return type;
  }

  // primary
  //   ::= identifierexpr
  //   ::= numberexpr
  //   ::= parenexpr
  //   ::= tensorliteral
  std::unique_ptr<Expr> Parser::parsePrimary() {
    switch (fLexer->getCurrentToken()) {
    default:
      std::cout << "unknown token '" << fLexer->getCurrentToken()
                   << "' when expecting an expression\n";
      return nullptr;
    case lexer::tok_identifier:
      return parseIdentifierExpr();
    case lexer::tok_number:
      return parseNumberExpr();
    case lexer::tok_paren_open:
      return parseParenExpr();
    case lexer::tok_sbracket_open:
      return parseTensorLiteralExpr();
    case lexer::tok_semicolon:
      return nullptr;
    case lexer::tok_bracket_close:
      return nullptr;
    }
  }

  // Recursively parse the right hand side of a binary expression, the ExprPrec
  // argument indicates the precedence of the current binary operator.
  //
  // binoprhs ::= ('+' primary)*
  std::unique_ptr<Expr> Parser::parseBinOpRHS(int aExprPrec, std::unique_ptr<Expr> lhs) {
    while (true) {
      int tokPrec = getTokPrecedence();

      // if this is a bunop that binds atleast as tightly as the current one consume it,
      // otherwise we are done
      if (tokPrec < aExprPrec)
        return lhs;

      // Okay, we know this is a binop.
      int binOp = fLexer->getCurrentToken();
      fLexer->consume(lexer::Token(binOp));
      auto loc = fLexer->getCurrentLocation();

      // Parse the primary expression after the binary operator.
      auto rhs = parsePrimary();
      if (!rhs) {
        return parseError<Expr>("expression", "to complete binary operator");
      }

      // If BinOp binds less tightly with rhs than the operator after rhs, let
      // the pending operator take rhs as its lhs.
      int nextPrec = getTokPrecedence();
      if (tokPrec < nextPrec) {
        rhs = parseBinOpRHS(tokPrec + 1, std::move(rhs));
        if (!rhs) {
          return nullptr;
        }
      }

      // Merge lhs/RHS.
      lhs = std::make_unique<BinaryExpr>(binOp, std::move(lhs), std::move(rhs), std::move(loc));
    }
  }

  // identifierexpr
  //   ::= identifier
  //   ::= identifier '(' expression ')'
  std::unique_ptr<Expr> Parser::parseIdentifierExpr() {
    std::string name(fLexer->getLiteral());

    auto loc = fLexer->getCurrentLocation();
    fLexer->consume(lexer::tok_identifier);

    if (fLexer->getCurrentToken() != lexer::tok_paren_open) // Simple variable ref.
      return std::make_unique<VarExpr>(name, std::move(loc));

    // This is a function call.
    fLexer->consume(lexer::tok_paren_open);
    std::vector<std::unique_ptr<Expr>> args;
    if (fLexer->getCurrentToken() != lexer::tok_paren_close) {
      while (true) {
        if (auto arg = parseExpression())
          args.push_back(std::move(arg));
        else
          return nullptr;

        if (fLexer->getCurrentToken() == lexer::tok_paren_close)
          break;

        if (fLexer->getCurrentToken() != lexer::tok_comma)
          return parseError<Expr>(", or )", "in argument list");
        fLexer->consume(lexer::tok_comma);
      }
    }
    fLexer->consume(lexer::tok_paren_close);

    // It can be a builtin call to print
    if (name == "print") {
      if (args.size() != 1)
        return parseError<Expr>("<single arg>", "as argument to print()");

      return std::make_unique<PrintExpr>(std::move(args[0]), std::move(loc));
    }

    // Call to a user-defined function
    return std::make_unique<CallExpr>(name, std::move(args), std::move(loc));
  }

  // Parse a literal number.
  // numberexpr ::= number
  std::unique_ptr<Expr> Parser::parseNumberExpr() {
    auto loc = fLexer->getCurrentLocation();
    auto result =
        std::make_unique<NumberExpr>(std::stoi(fLexer->getLiteral()), std::move(loc));
    fLexer->consume(lexer::tok_number);
    return std::move(result);
  }

  // parenexpr ::= '(' expression ')'
  std::unique_ptr<Expr> Parser::parseParenExpr() {
    fLexer->consume(lexer::tok_paren_open);
    auto v = parseExpression();
    if (!v)
      return nullptr;

    if (fLexer->getCurrentToken() != lexer::tok_paren_close)
      return parseError<Expr>(")", "to close expression with parentheses");
    fLexer->consume(lexer::tok_paren_close);
    return v;
  }

  // Parse a literal array expression.
  // tensorLiteral ::= [ literalList ] | number
  // literalList ::= tensorLiteral | tensorLiteral, literalList
  std::unique_ptr<Expr> Parser::parseTensorLiteralExpr() {
    auto loc = fLexer->getCurrentLocation();
    fLexer->consume(lexer::tok_sbracket_open);

    // Hold the list of values at this nesting level.
    ExprList values;
    // Hold the dimensions for all the nesting inside this level.
    std::vector<int> dims;
    do {
      // We can have either another nested array or a number literal.
      if (fLexer->getCurrentToken() == lexer::tok_sbracket_open) {
        values.push_back(parseTensorLiteralExpr());
        if (!values.back())
          return nullptr; // parse error in the nested array.
      } else {
        if (fLexer->getCurrentToken() != lexer::tok_number)
          return parseError<Expr>("<num> or [", "in literal expression");
        values.push_back(parseNumberExpr());
      }

      // End of this list on ']'
      if (fLexer->getCurrentToken() == lexer::tok_sbracket_close)
        break;

      // Elements are separated by a comma.
      if (fLexer->getCurrentToken() != lexer::tok_comma)
        return parseError<Expr>("] or ,", "in literal expression");

      fLexer->consume(lexer::tok_comma);
    } while (true);

    if (values.empty())
      return parseError<Expr>("<something>", "to fill literal expression");

    fLexer->consume(lexer::tok_sbracket_close);

    // Fill in the dimensions now. First the current nesting level:
    dims.push_back(values.size());

    // If there is any nested array, process all of them and ensure that
    // dimensions are uniform.
    auto literalFound = std::any_of(
        values.begin(), values.end(), [](const std::unique_ptr<Expr> &expr) {
          return dynamic_cast<LiteralExpr *>(expr.get()) != nullptr;
        });

    if (literalFound) {
      // Attempt to dynamically cast the first element to LiteralExpr*
      LiteralExpr *firstLiteral =
          dynamic_cast<LiteralExpr *>(values.front().get());
      if (!firstLiteral) {
        return parseError<Expr>("uniform well-nested dimensions",
                            "inside literal expression");
      }

      // Retrieve and append the nested dimensions to the current level
      auto firstDims = firstLiteral->getDims();
      std::vector<int>
          dims; // Assuming dimensions are represented as vector<int>
      dims.insert(dims.end(), firstDims.begin(), firstDims.end());

      // Ensure uniformity across all elements
      for (auto &expr : values) {
        LiteralExpr *exprLiteral =
            dynamic_cast<LiteralExpr *>(expr.get());
        if (!exprLiteral) {
          return parseError<Expr>("uniform well-nested dimensions",
                              "inside literal expression");
        }
        if (exprLiteral->getDims() != firstDims) {
          return parseError<Expr>("uniform well-nested dimensions",
                              "inside literal expression");
        }
      }
    }

    return std::make_unique<LiteralExpr>(std::move(values), std::move(dims), std::move(loc));
  }

  int Parser::getTokPrecedence() {
    if (!isascii(fLexer->getCurrentToken()))
      return -1;

    // 1 is lowest precedence.
    switch (static_cast<char>(fLexer->getCurrentToken())) {
    case '-':
      return 20;
    case '+':
      return 20;
    case '*':
      return 40;
    default:
      return -1;
    }
  }

  template <typename R, typename T, typename U>
  std::unique_ptr<R> Parser::parseError(T &&expected, U &&context) {
    auto curToken = fLexer->getCurrentToken();
    std::cout << "Parse error (" << fLexer->getCurrentLocation().line << ", "
                 << fLexer->getCurrentLocation().col << "): expected '" << expected
                 << "' " << context << " but has Token " << curToken;
    if (isprint(curToken))
      std::cout << " '" << (char)curToken << "'";
    std::cout << "\n";
    return nullptr;
  }  

}