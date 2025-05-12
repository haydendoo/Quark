#pragma once

#include <array>
#include <utility>
#include <unordered_map>
#include <string>

enum class TokenType {
    // Single character tokens
    LPAREN, RPAREN,
    LBRACE, RBRACE,
    LBRACKET, RBRACKET,
    EQUALS,
    LESS,
    GREATER,
    SEMICOLON,
    PLUS,
    MINUS,
    ASTERISK,
    SLASH,
    COMMA,
    EXCLAMATION_MARK,
    
    // Multi-character tokens
    EXPONENTIATION,
    EQUALS_EQUALS,
    NOT_EQUALS,
    LESS_EQUALS,
    GREATER_EQUALS,
    AND,
    OR,

    // End indicators
    END_OF_FILE,

    // Literals
    IDENTIFIER,
    // Numbers
    INTEGER,
    FLOAT,
    STRING,
    CHAR,
    BOOLEAN,

    // Keywords
    VOID_KEYWORD,
    INT_KEYWORD,
    FLOAT_KEYWORD,
    STRING_KEYWORD,
    CHAR_KEYWORD,
    BOOL_KEYWORD,
    IF_KEYWORD,
    ELSE_KEYWORD,
    WHILE_KEYWORD,
    FOR_KEYWORD,
    RETURN_KEYWORD,
    TRUE_KEYWORD,
    FALSE_KEYWORD,

    // Others
    INVALID_TOKEN
};

static const std::array<std::pair<std::string, TokenType>, 7> multi_char_token_map = {
    std::make_pair("**", TokenType::EXPONENTIATION),
    std::make_pair("==", TokenType::EQUALS_EQUALS),
    std::make_pair("!=", TokenType::NOT_EQUALS),
    std::make_pair("<=", TokenType::LESS_EQUALS),
    std::make_pair(">=", TokenType::GREATER_EQUALS),
    std::make_pair("&&", TokenType::AND),
    std::make_pair("||", TokenType::OR),
};

static const std::unordered_map<std::string, TokenType> keyword_map = {
    {"if", TokenType::IF_KEYWORD},
    {"else", TokenType::ELSE_KEYWORD},
    {"while", TokenType::WHILE_KEYWORD},
    {"for", TokenType::FOR_KEYWORD},
    {"return", TokenType::RETURN_KEYWORD},
    {"true", TokenType::TRUE_KEYWORD},
    {"false", TokenType::FALSE_KEYWORD},
    {"void", TokenType::VOID_KEYWORD},
    {"int", TokenType::INT_KEYWORD},
    {"float", TokenType::FLOAT_KEYWORD},
    {"string", TokenType::STRING_KEYWORD},
    {"char", TokenType::CHAR_KEYWORD},
    {"bool", TokenType::BOOL_KEYWORD},
};