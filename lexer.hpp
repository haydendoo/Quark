#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

enum TokenType {
	TokenType_IDENTIFIER,

	// Literals
	TokenType_DIGIT,
	TokenType_FLOAT,
	TokenType_STRING,
	TokenType_BOOL,

	// Operators
	TokenType_PLUS,
	TokenType_MINUS,
	TokenType_TIMES,
	TokenType_SLASH,
	TokenType_PERIOD,
	TokenType_EQL,
	TokenType_NOT,
	TokenType_LSS,
	TokenType_GTR,
	TokenType_LEQ,
	TokenType_GEQ,

	// Seperators
	TokenType_LPAREN,
	TokenType_RPAREN,
	TokenType_LBRACK,
	TokenType_RBRACK,
	TokenType_LBRACE,
	TokenType_RBRACE,
	TokenType_SEMICOLON,
	TokenType_COMMA,
	TokenType_BECOMES,

	// Keywords
	TokenType_IF,
	TokenType_WHILE,
	TokenType_FOR,
	TokenType_RETURN,
	TokenType_NEW,
	TokenType_DELETE,
	TokenType_NULL,

	// Unique
	TokenType_UNKNOWN,
	TokenType_EOF
};

struct Token {
    TokenType type;
    std::string data;
};

struct Tokenizer {
    std::string data;
    size_t index;
};

inline bool IsWhiteSpace(const char p_c);
inline bool IsEndOfLine(const char p_c);
inline bool IsLetter(const char p_c);
void IgnoreCommentsAndWhiteSpace(Tokenizer &tokenizer);
Token GetToken(Tokenizer &Tokenizer);
std::vector<Token> LexInput(std::string input);
std::string TokenTypeToString(const TokenType type);
void DebugPrintTokenArray(const std::vector<Token> &token_array);
