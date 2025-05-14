#pragma once

#include <string>
#include <utility>
#include <memory>
#include <vector>


// Interface class for an expression AST
class ExprAst {
public:
    ExprAst() = default;
    virtual ~ExprAst() = default;

    ExprAst(const ExprAst &exp) = default;
    auto operator=(const ExprAst &other) -> ExprAst& = default;
    ExprAst(ExprAst&& other) = default;
    auto operator=(ExprAst&& other) -> ExprAst& = default;
};


// For numbers
class NumberExprAst : public ExprAst {
private:
    double m_val;

public:
    NumberExprAst(double val) : m_val(val) {}
};


// For variables
class VariableExprAst : public ExprAst {
private:
    std::string m_name;

public:
    VariableExprAst(std::string name): m_name(std::move(name)) {}
};


// For binary operators 
class BinaryExprAst : public ExprAst {
private:
    char m_operator;
    std::unique_ptr<ExprAst> m_LHS, m_RHS;

public:
    BinaryExprAst(char oper, std::unique_ptr<ExprAst> LHS, 
                  std::unique_ptr<ExprAst> RHS)
    : m_operator(oper), m_LHS(std::move(LHS)), m_RHS(std::move(RHS)) {}
};


// For a function call
class CallExprAst : public ExprAst {
private:
    std::string m_callee;
    std::vector<std::unique_ptr<ExprAst>> m_args;

public:
    CallExprAst(std::string callee, std::vector<std::unique_ptr<ExprAst>> args)
    : m_callee(std::move(callee)), m_args(std::move(args)) {}
};


// The "schema" of a function (name, args, etc.)
class PrototypeAst : public ExprAst {
private:
    std::string m_name;
    std::vector<std::string> m_args;

public:
    PrototypeAst(std::string name, std::vector<std::string> args)
    : m_name(std::move(name)), m_args(std::move(args)) {}
};


// Actual function with prototype and body itself
class FunctionAst : public ExprAst {
private:
    std::unique_ptr<PrototypeAst> m_prototype;
    std::unique_ptr<ExprAst> m_body;

public:
    FunctionAst(std::unique_ptr<PrototypeAst> prototype, std::unique_ptr<ExprAst> body)
    : m_prototype(std::move(prototype)), m_body(std::move(body)) {}
};
