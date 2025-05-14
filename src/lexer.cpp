#include "lexer.hpp"
#include "constants.hpp"
#include "utils.hpp"

#include <stdexcept>
#include <string>
#include <cctype>
#include <unordered_map>

auto token_to_string(const TokenType token) -> std::string {
    switch (token) {
        case TokenType::LPAREN: return "(";
        case TokenType::RPAREN: return ")";
        case TokenType::LBRACE: return "{";
        case TokenType::RBRACE: return "}";
        case TokenType::LBRACKET: return "[";
        case TokenType::RBRACKET: return "]";
        case TokenType::EQUALS: return "=";
        case TokenType::LESS: return "<";
        case TokenType::GREATER: return ">";
        case TokenType::SEMICOLON: return ";";
        case TokenType::PLUS: return "+";
        case TokenType::MINUS: return "-";
        case TokenType::ASTERISK: return "*";
        case TokenType::SLASH: return "/";
        case TokenType::COMMA: return ",";
        case TokenType::EXCLAMATION_MARK: return "!";
        case TokenType::EXPONENTIATION: return "**";
        case TokenType::EQUALS_EQUALS: return "==";
        case TokenType::NOT_EQUALS: return "!=";
        case TokenType::LESS_EQUALS: return "<=";
        case TokenType::GREATER_EQUALS: return ">=";
        case TokenType::AND: return "&&";
        case TokenType::OR: return "||";
        case TokenType::END_OF_FILE: return "EOF";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::INTEGER: return "INTEGER";
        case TokenType::FLOAT: return "FLOAT";
        case TokenType::STRING: return "STRING";
        case TokenType::CHAR: return "CHAR";
        case TokenType::BOOLEAN: return "BOOLEAN";
        case TokenType::FUNC_KEYWORD: return "func";
        case TokenType::VOID_KEYWORD: return "void";
        case TokenType::IMPORT_KEYWORD: return "import";
        case TokenType::INT_KEYWORD: return "int";
        case TokenType::FLOAT_KEYWORD: return "float";
        case TokenType::STRING_KEYWORD: return "string";
        case TokenType::CHAR_KEYWORD: return "char";
        case TokenType::BOOL_KEYWORD: return "bool";
        case TokenType::IF_KEYWORD: return "if";
        case TokenType::ELSE_KEYWORD: return "else";
        case TokenType::WHILE_KEYWORD: return "while";
        case TokenType::FOR_KEYWORD: return "for";
        case TokenType::RETURN_KEYWORD: return "return";
        case TokenType::TRUE_KEYWORD: return "true";
        case TokenType::FALSE_KEYWORD: return "false";
        case TokenType::INVALID_TOKEN: [[unlikely]] return "INVALID_TOKEN";
        default: [[unlikely]]
            throw std::runtime_error("Unknown token type");
    }
}

inline void Lexer::m_skip_whitespace() noexcept {
    while (m_pos < m_source.size() && isspace(m_source[m_pos]) != 0) {
        ++m_pos;
    }
}

inline void Lexer::m_skip_inline_comment() noexcept {
    while (m_pos < m_source.size() && m_source[m_pos] != '\n') {
        ++m_pos;
    }
}

inline void Lexer::m_skip_block_comment() {
    while (m_pos+1 < m_source.size() && (m_source[m_pos] != '*' || m_source[m_pos+1] != '/')) {
        ++m_pos;
    }
    if (m_pos+1 < m_source.size() && m_source[m_pos] == '*' && m_source[m_pos+1] == '/') {
        m_pos += 2;
        return;
    }
    throw std::runtime_error("Unterminated block comment");
}

void Lexer::m_skip_whitespace_and_comments() {
    while(true) {
        m_skip_whitespace();
        if(m_pos + 2 < m_source.size() && m_source.compare(m_pos, 2, "//") == 0) {
            m_skip_inline_comment();
        }
        else if(m_pos + 2 < m_source.size() && m_source.compare(m_pos, 2, "/*") == 0) {
            m_skip_block_comment();
        }
        else {
            break;
        }
    }
}

auto Lexer::m_read_single_char_token() noexcept -> Token {
    switch(m_source[m_pos++]) {
        case '(': return {.type = TokenType::LPAREN, .value="("};
        case ')': return {.type = TokenType::RPAREN, .value=")"};
        case '{': return {.type = TokenType::LBRACE, .value="{"};
        case '}': return {.type = TokenType::RBRACE, .value="}"};
        case '[': return {.type = TokenType::LBRACKET, .value="["};
        case ']': return {.type = TokenType::RBRACKET, .value="]"};
        case ',': return {.type = TokenType::COMMA, .value=","};
        case ';': return {.type = TokenType::SEMICOLON, .value=";"};
        case '+': return {.type = TokenType::PLUS, .value="+"};
        case '-': return {.type = TokenType::MINUS, .value="-"};
        case '*': return {.type = TokenType::ASTERISK, .value="*"};
        case '/': {
            if (m_pos < m_source.size() && m_source[m_pos] != '/' && m_source[m_pos] != '*') {
                return {.type = TokenType::SLASH, .value="/"};
            }
            break;
        }
        case '=': return {.type = TokenType::EQUALS, .value="="};
        case '<': return {.type = TokenType::LESS, .value="<"};
        case '>': return {.type = TokenType::GREATER, .value=">"};
        case '!': return {.type = TokenType::EXCLAMATION_MARK, .value="!"};
        default:
            --m_pos;
            break;
    }
    return {.type = TokenType::INVALID_TOKEN, .value=""};
}

auto Lexer::m_read_multi_char_token() noexcept -> Token {
    // Checks for multi-character tokens like **, ==, !=, <=, >=, &&, ||, etc.
    for(const auto& [str, token_type] : multi_char_token_map) {
        if(m_source.compare(m_pos, str.size(), str) == 0) {
            m_pos += str.size();
            return {.type = token_type, .value=str};
        }
    }
    [[likely]]
    return {.type = TokenType::INVALID_TOKEN, .value=""};
}

auto Lexer::m_read_number() noexcept -> Token {
    std::string number(1, m_source[m_pos]);
    bool has_decimal = false;
        
    ++m_pos;
    while (m_pos < m_source.size()) {
        const char next = m_source[m_pos];
        if (isdigit(next) != 0) {
            number += next;
        }
        else if (next == '.' && !has_decimal) {
            number += next;
            has_decimal = true;
        }
        else {
            break;
        }
        ++m_pos;
    }
        
    // Ensure we don't end with a decimal point
    if (number.back() == '.') {
        number.pop_back();
    }
        
    if (has_decimal) {
        return {.type = TokenType::FLOAT, .value=number};
    }
    return {.type = TokenType::INTEGER, .value=number};
}

auto Lexer::m_read_identifier_or_keyword() noexcept -> Token {
    std::string identifier(1, m_source[m_pos]);
    ++m_pos;

    while (m_pos < m_source.size() && (isalnum(m_source[m_pos]) != 0 || m_source[m_pos] == '_')) {
        identifier += m_source[m_pos];
        ++m_pos;
    }
        
    // Check if it's a keyword
    auto keyword = keyword_map.find(identifier);
    if(keyword != keyword_map.end()) {
        return {.type = keyword->second, .value=identifier};
    }

    return {.type = TokenType::IDENTIFIER, .value=identifier};
}

auto Lexer::m_read_string() -> Token {
    std::string str;
    ++m_pos;
    while (m_pos < m_source.size() && m_source[m_pos] != '"') {
        str += m_source[m_pos];
        ++m_pos;
    }

    if (m_pos < m_source.size() && m_source[m_pos] == '"') {
        [[likely]]
        ++m_pos;
        return {.type = TokenType::STRING, .value=str};
    }
    
    [[unlikely]]
    throw std::runtime_error("Unterminated string");
}

auto Lexer::m_read_char() -> Token {
    if (m_pos+2 < m_source.size() && m_source[m_pos+2] == '\'') {
        [[likely]]
        m_pos += 3;
        return {.type = TokenType::CHAR, .value=std::string(1, m_source[m_pos-2])};
    }

    [[unlikely]]
    throw std::runtime_error("Unterminated character");
}

auto Lexer::m_read_literal() -> Token {
    if(isdigit(m_source[m_pos]) != 0) {
        return m_read_number();
    }
    if(isalpha(m_source[m_pos]) != 0 || m_source[m_pos] == '_') {
        return m_read_identifier_or_keyword();
    }
    if(m_source[m_pos] == '"') {
        return m_read_string();
    }
    if(m_source[m_pos] == '\'') {
        return m_read_char();
    }
    return {.type = TokenType::INVALID_TOKEN, .value=""};
}

auto Lexer::get_next_token() -> Token {
    m_skip_whitespace_and_comments();

    auto *logger = QuarkLogger::get_instance();

    if (m_pos >= m_source.size()) {
        logger->info("Finished lexing source code");
        return {.type = TokenType::END_OF_FILE, .value=""};
    }

    Token token = m_read_multi_char_token();
    if(token.type != TokenType::INVALID_TOKEN) {
        logger->info("Found multi character token of type: " + token_to_string(token.type));
        return token;
    }

    token = m_read_single_char_token();
    if(token.type != TokenType::INVALID_TOKEN) {
        logger->info("Found single character token of type: " + token_to_string(token.type));
        return token;
    }

    token = m_read_literal();
    if(token.type != TokenType::INVALID_TOKEN) {
        logger->info("Found literal of type: " + token_to_string(token.type));
        return token;
    }

    // If we get here, we found an invalid token
    [[unlikely]]
    logger->error("Found an invalid token");
    throw std::runtime_error("Syntax error");
}
