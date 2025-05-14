#include "parser.hpp"
#include "constants.hpp"

#include <unordered_map>
#include <memory>
#include <cstdint>

const std::unordered_map<TokenType, uint8_t> QuarkParser::m_binop_priority = {
    { TokenType::EXPONENTIATION, 70 },
    { TokenType::ASTERISK, 60 },
    { TokenType::SLASH, 60 },
    { TokenType::PLUS, 50 },
    { TokenType::MINUS, 50 },
    { TokenType::EQUALS_EQUALS, 40 },
    { TokenType::NOT_EQUALS, 40 },
    { TokenType::LESS_EQUALS, 40 },
    { TokenType::LESS, 40 },
    { TokenType::GREATER_EQUALS, 40 },
    { TokenType::GREATER, 40 },
    { TokenType::EXCLAMATION_MARK, 30 },
    { TokenType::AND, 20 },
    { TokenType::OR, 10 },
};

auto QuarkParser::get_token_priority(const TokenType &type) -> uint8_t {
    auto iterator = m_binop_priority.find(type);
    if(iterator == m_binop_priority.end()) {
        return 0;
    }
    return iterator->second;
}

inline void QuarkParser::advance() {
    m_current_token = m_lexer->get_next_token();
}

void QuarkParser::parse_code() {
    while(true) {
        switch(m_current_token.type) {
            case TokenType::END_OF_FILE: [[unlikely]] return;
            case TokenType::SEMICOLON:
                advance();
                break;
            default:
                break;
        }
    }
}
