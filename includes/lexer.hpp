#pragma once

#include "constants.hpp"
#include <string>

struct Token {
    TokenType type;
    std::string value;
};

auto token_to_string(TokenType token) -> std::string;
class Lexer {
private:
    std::string m_source;
    size_t m_pos = 0;

    inline void m_skip_whitespace() noexcept;
    inline void m_skip_inline_comment() noexcept;
    inline void m_skip_block_comment();
    void m_skip_whitespace_and_comments();
    auto m_read_single_char_token() noexcept -> Token;
    auto m_read_multi_char_token() noexcept -> Token;

    // Literal functions
    auto m_read_literal() -> Token;
    auto m_read_number() noexcept -> Token;
    auto m_read_identifier_or_keyword() noexcept -> Token;
    auto m_read_string() -> Token;
    auto m_read_char() -> Token;

public:
    Lexer() = default;
    Lexer(std::string &source): m_source(std::move(source)) {}

    auto get_next_token() -> Token;
};