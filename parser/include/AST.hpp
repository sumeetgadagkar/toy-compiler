/**
 * Abstract Syntax Tree
 */

#pragma once

#include "lexer/include/AbstractLexer.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace toy {

class Expr {
public:
  Expr(lexer::Location aLoc) : fLoc(std::move(aLoc)) {}

  virtual ~Expr() = default;

  const lexer::Location &getLoc() { return fLoc; }

private:
  lexer::Location fLoc;
};

using ExprList = std::vector<std::unique_ptr<Expr>>;
using Shape = std::vector<int>;

class NumberExpr : public Expr {
public:
  NumberExpr(double aVal, lexer::Location aLoc)
      : Expr(std::move(aLoc)), fVal(aVal) {}

  const double &getValue() { return fVal; }

private:
  double fVal;
};

class LiteralExpr : public Expr {
public:
  LiteralExpr(ExprList vals, std::vector<int> &dims, lexer::Location aLoc)
      : Expr(std::move(aLoc)), fVals(std::move(vals)), fDims(std::move(dims)) {}

  const ExprList &getValues() { return fVals; }

  const std::vector<int> &getDims() { return fDims; }

private:
  ExprList fVals;
  std::vector<int> fDims;
};

class VarExpr : public Expr {
public:
  VarExpr(const std::string &aName, lexer::Location aLoc)
      : Expr(std::move(aLoc)), fName(aName) {}

  const std::string &getName() { return fName; }

private:
  std::string fName;
};

class VarDeclExpr : public Expr {
public:
  VarDeclExpr(const std::string &aName, const Shape &aShape,
              std::unique_ptr<Expr> aInitVal, lexer::Location aLoc)
      : Expr(std::move(aLoc)), fName(aName), fShape(aShape),
        fInitVal(std::move(aInitVal)) {}

  const std::string &getName() { return fName; }

  const Shape &getShape() { return fShape; }

  Expr *getInitValue() { return fInitVal.get(); }

private:
  std::string fName;
  Shape fShape;
  std::unique_ptr<Expr> fInitVal;
};

class ReturnExpr : public Expr {
public:
  ReturnExpr(std::unique_ptr<Expr> aExpr, lexer::Location aLoc)
      : Expr(std::move(aLoc)), fExpr(std::move(aExpr)) {}

  std::optional<Expr *> getExpr() {
    if (fExpr.has_value()) {
      return fExpr->get();
    }
    return std::nullopt;
  }

private:
  std::optional<std::unique_ptr<Expr>> fExpr;
};

class BinaryExpr : public Expr {
public:
  BinaryExpr(char aOp, std::unique_ptr<Expr> aLHS, std::unique_ptr<Expr> aRHS,
             lexer::Location aLoc)
      : Expr(std::move(aLoc)), fOp(aOp), fLHS(std::move(aLHS)),
        fRHS(std::move(aRHS)) {}

  char getOp() { return fOp; }

  Expr *getLHS() { return fLHS.get(); }

  Expr *getRHS() { return fRHS.get(); }

private:
  char fOp;
  std::unique_ptr<Expr> fLHS;
  std::unique_ptr<Expr> fRHS;
};

class CallExpr : public Expr {
public:
  CallExpr(const std::string &aCallee, ExprList args, lexer::Location aLoc)
      : Expr(std::move(aLoc)), fCallee(aCallee), fArgs(std::move(args)) {}

  const std::string &getCallee() { return fCallee; }

  const ExprList &getArgs() { return fArgs; }

private:
  std::string fCallee;
  ExprList fArgs;
};

class PrintExpr : public Expr {
public:
  PrintExpr(std::unique_ptr<Expr> aExpr, lexer::Location aLoc)
      : Expr(std::move(aLoc)), fArg(std::move(aExpr)) {}

  Expr *getArg() { return fArg.get(); }

private:
  std::unique_ptr<Expr> fArg;
};

class Prototype : public Expr {
public:
  Prototype(const std::string &aName, std::vector<std::unique_ptr<VarExpr>> args, lexer::Location aLoc)
      : Expr(std::move(aLoc)), fName(aName), fArgs(std::move(args)) {}

  const std::string &getName() { return fName; }

  const std::vector<std::unique_ptr<VarExpr>> &getArgs() { return fArgs; }

private:
  std::string fName;
  std::vector<std::unique_ptr<VarExpr>> fArgs;
};

class Function {
public:
  Function(std::unique_ptr<Prototype> aPrototype,
           std::unique_ptr<ExprList> aBody)
      : fProto(std::move(aPrototype)),
        fBody(std::move(aBody)) {}

  Prototype *getPrototype() { return fProto.get(); }

  ExprList *getBody() { return fBody.get(); }

private:
  std::unique_ptr<Prototype> fProto;
  std::unique_ptr<ExprList> fBody;
};

class Module {
  public:
    Module(std::vector<std::unique_ptr<Function>> functions) : fFunctions(std::move(functions)) {}

    auto begin() { return fFunctions.begin();}

    auto end() { return fFunctions.end();}

  private:
    std::vector<std::unique_ptr<Function>> fFunctions;
};

void dump(Module& aMod);

} // namespace toy
