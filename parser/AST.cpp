#include "parser/include/AST.hpp"

#include <sstream>
#include <string>
#include <cassert>

namespace toy {
  
  struct Indent {
    Indent(int &level) : fLevel(level) { ++fLevel;}
    ~Indent() {--fLevel;}
    int& fLevel;
  };

  // convert location to a string
  template <typename T>
  static std::string getLocStr(T *aExpr) {
    const lexer::Location& loc = aExpr->getLoc();
    return ("@" + *(loc.file) + ":" + std::to_string(loc.line) + ":" + std::to_string(loc.col));
  }

  class ASTDumper {
    public:
      // public API to dump a module
      void dump(Module *aMod);
    
    private:
      void dump(NumberExpr *aNumberExpr);
      void dump(LiteralExpr *aLiteralExpr);
      void dump(VarExpr *aVarExpr);
      void dump(VarDeclExpr *aVarDeclExpr);
      void dump(ReturnExpr *aReturnExpr);
      void dump(BinaryExpr *aBinaryExpr);
      void dump(CallExpr *aCallExpr);
      void dump(PrintExpr *aPrintExpr);
      void dump(Prototype *aPrototype);
      void dump(Function *aFunction);
      void dump(ExprList *aExprList);
      void dump(const VarType& aType);
      // generic Expr printer, dipatches to correct method w.r.t subclass
      void dump(Expr *aExpr);
      // function to print out current indentation level
      void indent();
      // literal / num print helper
      void printLitHelper(Expr *aLiteralOrNumExpr);

      int fCurrIndent = 0;
      std::ostringstream fOss;
  };

  void ASTDumper::indent() {
    for (int i = 0; i < fCurrIndent; i++) {
      fOss << "  ";
    }
  }

  // indent macro to be used inside ASTDumper
  #define INDENT()                            \
    Indent level_(fCurrIndent);               \
    indent();

  void ASTDumper::dump(Expr *aExpr) {
    // dispatch to correct dump function
    if (NumberExpr* expr = dynamic_cast<NumberExpr*>(aExpr)) {
      ASTDumper::dump(expr);
    }
    else if (LiteralExpr* expr = dynamic_cast<LiteralExpr*>(aExpr)) {
      ASTDumper::dump(expr);
    }
    else if (VarExpr* expr = dynamic_cast<VarExpr*>(aExpr)) {
      ASTDumper::dump(expr);
    }
    else if (VarDeclExpr* expr = dynamic_cast<VarDeclExpr*>(aExpr)) {
      ASTDumper::dump(expr);
    }
    else if (ReturnExpr* expr = dynamic_cast<ReturnExpr*>(aExpr)) {
      ASTDumper::dump(expr);
    }
    else if (BinaryExpr* expr = dynamic_cast<BinaryExpr*>(aExpr)) {
      ASTDumper::dump(expr);
    }
    else if (CallExpr* expr = dynamic_cast<CallExpr*>(aExpr)) {
      ASTDumper::dump(expr);
    }
    else if (PrintExpr* expr = dynamic_cast<PrintExpr*>(aExpr)) {
      ASTDumper::dump(expr);
    }
    else if (Prototype* expr = dynamic_cast<Prototype*>(aExpr)) {
      ASTDumper::dump(expr);
    }
    else if (Function* expr = dynamic_cast<Function*>(aExpr)) {
      ASTDumper::dump(expr);
    }
    else {
      fOss << std::string("Unknown expr of type ") + typeid(*aExpr).name() << std::endl;
    }
  }

  void ASTDumper::dump(NumberExpr *aNumberExpr) {
    INDENT();
    fOss << std::to_string(aNumberExpr->getValue()) << " " << getLocStr(aNumberExpr) << std::endl;
  }

  void ASTDumper::dump(ExprList *aExprList) {
    INDENT();
    fOss << "Block {" << std::endl;
    for (auto &expr : *aExprList) {
      dump(expr.get());
    }
    indent();
    fOss << "// Block" << std::endl;
  }

  void ASTDumper::printLitHelper(Expr *aLiteralOrNumExpr) {
    // check if number or another literal
    if (auto *numExpr = dynamic_cast<NumberExpr*>(aLiteralOrNumExpr)) {
      // number
      fOss << numExpr->getValue();
      return;
    }

    // literal
    auto *litExpr = dynamic_cast<LiteralExpr*>(aLiteralOrNumExpr);
    assert(litExpr != nullptr);

    // print dims
    fOss << "<";
    auto &dims = litExpr->getDims();
    for (auto &dim : dims) {
      fOss << dim << ",";
    }
    fOss << ">";

    // print contents
    fOss << "[";
    for (const auto& expr : litExpr->getValues()) {
      printLitHelper(expr.get());
      fOss << ",";
    }
    fOss << "]";
  }

  void ASTDumper::dump(LiteralExpr *aLiteralExpr) {
    INDENT();
    fOss << "Literal: ";
    printLitHelper(aLiteralExpr);
    fOss << " " << getLocStr(aLiteralExpr) << std::endl;
  }

  void ASTDumper::dump(VarExpr *aVarExpr) {
    INDENT();
    fOss << "Var: " << aVarExpr->getName() << " " << getLocStr(aVarExpr) << std::endl;
  }

  void ASTDumper::dump(VarDeclExpr *aVarDeclExpr) {
    INDENT();
    fOss << "VarDecl: " << aVarDeclExpr->getName(); 
    dump(aVarDeclExpr->getType());
    fOss << " " << getLocStr(aVarDeclExpr) << std::endl;
    dump(aVarDeclExpr->getInitValue());
  }

  void ASTDumper::dump(ReturnExpr *aReturnExpr) {
    INDENT();
    fOss << "Return" << std::endl;
    if (aReturnExpr->getExpr().has_value()) {
      return dump(*aReturnExpr->getExpr());
    }
    {
      INDENT();
      fOss << "(void)" << std::endl;
    }
  }

  void ASTDumper::dump(BinaryExpr *aBinaryExpr) {
    INDENT();
    fOss << "BinOp: " << aBinaryExpr->getOp() << " " << getLocStr(aBinaryExpr) << std::endl;
    dump(aBinaryExpr->getLHS());
    dump(aBinaryExpr->getRHS());
  }

  void ASTDumper::dump(CallExpr *aCallExpr) {
    INDENT();
    fOss << "Call '" << aCallExpr->getCallee() << "' [ " << getLocStr(aCallExpr) << std::endl;
    for (auto &arg : aCallExpr->getArgs()) {
      dump(arg.get());
      fOss << ",";
    }
    indent();
    fOss << "]" << std::endl;
  }

  void ASTDumper::dump(PrintExpr *aPrintExpr) {
    INDENT();
    fOss << "Print [ " << getLocStr(aPrintExpr) << std::endl;
    dump(aPrintExpr->getArg());
    indent();
    fOss << "]" << std::endl;
  }

  void ASTDumper::dump(const VarType& aType) {
    fOss << "<";
    for (auto &dim : aType.shape) {
      fOss << dim << ",";
    }
    fOss << ">";
  }

  void ASTDumper::dump(Prototype *aPrototype) {
    INDENT();
    fOss << "Proto '" << aPrototype->getName() << "' " << getLocStr(aPrototype) << std::endl;
    indent();
    fOss << "Params: [";
    for (auto &arg : aPrototype->getArgs()) {
      fOss << arg->getName() << ",";
    }
    fOss << "]" << std::endl;
  }

  void ASTDumper::dump(Function *aFunction) {
    INDENT();
    fOss << "Function " << std::endl;
    dump(aFunction->getPrototype());
    dump(aFunction->getBody());  
  }

  void ASTDumper::dump(Module *aModule) {
    INDENT();
    fOss << "Module: " << std::endl;
    for (auto &func : *aModule) {
      dump(func.get());
    }
  }

  void dump(Module &aModule) {
    ASTDumper().dump(&aModule);
  }
 
} // namespace toy