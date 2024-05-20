#include <iostream>

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
    auto loc = fLexer->getCurrentLocation();

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



  }


  std::unique_ptr<ExprList> Parser::parseBlock() {

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