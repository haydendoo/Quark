#pragma once

#include "constants.hpp"
#include "lexer.hpp"

#include <llvm/IR/Value.h>

#include <string>
#include <utility>
#include <memory>
#include <vector>
#include <unordered_map>
#include <cstdint>

// Interface class for an expression AST
class ExprAst {
public:
    ExprAst() = default;
    virtual auto generate_code() -> llvm::Value*;

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
    auto generate_code() -> llvm::Value* override;
};


// For variables
class VariableExprAst : public ExprAst {
private:
    std::string m_name;

public:
    VariableExprAst(std::string name): m_name(std::move(name)) {}
    auto generate_code() -> llvm::Value* override;
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

    auto generate_code() -> llvm::Value* override;
};


// For a function call
class CallExprAst : public ExprAst {
private:
    std::string m_callee;
    std::vector<std::unique_ptr<ExprAst>> m_args;

public:
    CallExprAst(std::string callee, std::vector<std::unique_ptr<ExprAst>> args)
    : m_callee(std::move(callee)), m_args(std::move(args)) {}

    auto generate_code() -> llvm::Value* override;
};


// The "schema" of a function (name, args, etc.)
class PrototypeAst : public ExprAst {
private:
    std::string m_name;
    std::vector<std::string> m_args;

public:
    PrototypeAst(std::string name, std::vector<std::string> args)
    : m_name(std::move(name)), m_args(std::move(args)) {}

    auto generate_code() -> llvm::Value* override;
};


// Actual function with prototype and body itself
class FunctionAst : public ExprAst {
private:
    std::unique_ptr<PrototypeAst> m_prototype;
    std::unique_ptr<ExprAst> m_body;

public:
    FunctionAst(std::unique_ptr<PrototypeAst> prototype, std::unique_ptr<ExprAst> body)
    : m_prototype(std::move(prototype)), m_body(std::move(body)) {}

    auto generate_code() -> llvm::Value* override;
};

// Imports
class ImportAst : public ExprAst {
private:
    std::string m_import;

public:
    ImportAst(std::string import): m_import(std::move(import)) {}
    auto generate_code() -> llvm::Value* override;
};


// Ast for a module (a source code file)
class ModuleAst : public ExprAst {
private:
    std::vector<std::unique_ptr<PrototypeAst>> m_prototypes;
    std::vector<std::unique_ptr<ImportAst>> m_imports;
    std::vector<std::unique_ptr<VariableExprAst>> m_global_variables;

public:
    ModuleAst(std::vector<std::unique_ptr<PrototypeAst>> prototypes,
              std::vector<std::unique_ptr<ImportAst>> imports,
              std::vector<std::unique_ptr<VariableExprAst>> global_variables) 

    : m_prototypes(std::move(prototypes)),
      m_imports(std::move(imports)),
      m_global_variables(std::move(global_variables)) {}

    auto generate_code() -> llvm::Value* override;
};


// Actual Parser (works with 1 file only for now)
class QuarkParser {
private:
    static const std::unordered_map<TokenType, uint8_t> m_binop_priority;
    std::unique_ptr<Lexer> m_lexer;
    std::unique_ptr<ModuleAst> m_module_ast;
    Token m_current_token;

    static auto get_token_priority(const TokenType &type) -> uint8_t;

    inline void advance();
    auto parse_number() -> std::unique_ptr<ExprAst>;
    auto parse_paren_expr() -> std::unique_ptr<ExprAst>;
    auto parse_identifier() -> std::unique_ptr<ExprAst>;
    auto parse_primary() -> std::unique_ptr<ExprAst>;
    auto parse_binop_rhs(int expr_prec, std::unique_ptr<ExprAst> LHS) -> std::unique_ptr<ExprAst>;
    auto parse_expression() -> std::unique_ptr<ExprAst>;
    auto parse_prototype() -> std::unique_ptr<PrototypeAst>;
    auto parse_function() -> std::unique_ptr<FunctionAst>;
    auto parse_import() -> std::unique_ptr<ImportAst>;

    void parse_top_level_exp();

public:
    QuarkParser(std::unique_ptr<Lexer> lexer): m_lexer(std::move(lexer)), m_module_ast(nullptr) {}
    void parse_code();
};
