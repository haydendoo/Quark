#include "lexer.hpp"

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

inline bool IsWhiteSpace(const char p_c) {
	return p_c == ' ' || p_c == '\t' || p_c == '\f' || p_c == '\v' || p_c == '\n';
}

inline bool IsEndOfLine(const char p_c) {
    return p_c == '\n' || p_c == '\r';
}

inline bool IsLetter(const char p_c) {
    return (p_c >= 'a' && p_c <= 'z') || (p_c >= 'A' && p_c <= 'Z');
}

inline bool IsNumeric(const char p_c) {
	return p_c >= '0' && p_c <= '9';
}

void IgnoreCommentsAndWhiteSpace(Tokenizer &tokenizer) {
    while(tokenizer.index < tokenizer.data.size() && IsWhiteSpace(tokenizer.data[tokenizer.index])) ++tokenizer.index;
    if(tokenizer.index == tokenizer.data.size()) return;
    
    // @ for single line and @@ for multi line comment
    if(tokenizer.index + 1 < tokenizer.data.size() && 
       tokenizer.data[tokenizer.index] == '@' && 
       tokenizer.data[tokenizer.index] == '@') {
        tokenizer.index += 2;
        while(
            tokenizer.index + 1 < tokenizer.data.size() && 
            (tokenizer.data[tokenizer.index] != '@' || 
            tokenizer.data[tokenizer.index+1] != '@')
        ) 
            ++tokenizer.index;
        
        tokenizer.index += 2;
        tokenizer.index = std::min(tokenizer.index, tokenizer.data.size());
        return;
    }
        
    if(tokenizer.data[tokenizer.index] == '@') {
        ++tokenizer.index;
        while(tokenizer.index < tokenizer.data.size() && !IsEndOfLine(tokenizer.data[tokenizer.index])) ++tokenizer.index;
        ++tokenizer.index;
        tokenizer.index = std::min(tokenizer.index, tokenizer.data.size());
    }
}

Token GetToken(Tokenizer &tokenizer) {
    Token token;
    
    IgnoreCommentsAndWhiteSpace(tokenizer);

    if(tokenizer.index >= tokenizer.data.size()) {
        token.type = TokenType_EOF;
        return token;
    }
    
    switch(tokenizer.data[tokenizer.index]) {
        /*
        Features yet to add
        1) \ charater for special characters
        */
        case '\0': token.type = TokenType_EOF; break;
    
    	case '(': token.type = TokenType_LPAREN; break;
    	case ')': token.type = TokenType_RPAREN; break;
    	case '{': token.type = TokenType_LBRACE; break;
    	case '}': token.type = TokenType_RBRACE; break;
    	case '[': token.type = TokenType_LBRACK; break;
    	case ']': token.type = TokenType_RBRACK; break;
    
    	case '+': token.type = TokenType_PLUS; break;
    	case '-': token.type = TokenType_MINUS; break;
    	case '*': token.type = TokenType_TIMES; break;
    	case '/': token.type = TokenType_SLASH; break;
    
    	case ';': token.type = TokenType_SEMICOLON; break;
    	case ',': token.type = TokenType_COMMA; break;
    	case '.': token.type = TokenType_PERIOD; break;
    
    	case '=': token.type = TokenType_EQL; break;
    	case '>': token.type = TokenType_GTR; break;
    	case '<': token.type = TokenType_LSS; break;
    	case '!': token.type = TokenType_NOT; break;
    
        case '`': {
            ++tokenizer.index;
            token.type = TokenType_STRING;
            
            while(tokenizer.data[tokenizer.index] != '`') {
                token.data.push_back(tokenizer.data[tokenizer.index]);
                ++tokenizer.index;
                if(tokenizer.index == tokenizer.data.size()) {
                    std::cout << "Quotations not closed properly" << std::endl;
                    token.type = TokenType_UNKNOWN;
                    break;
                }
            }
            break;
        }

        default: {
            if(IsLetter(tokenizer.data[tokenizer.index])) {
                token.type = TokenType_IDENTIFIER;

                while(IsLetter(tokenizer.data[tokenizer.index]) || IsNumeric(tokenizer.data[tokenizer.index]) || tokenizer.data[tokenizer.index] == '_') {
                    token.data.push_back(tokenizer.data[tokenizer.index]);
                    ++tokenizer.index;
                }

                --tokenizer.index;
    
                if(token.data == "for") {
                    token.type = TokenType_FOR;
                    break;
                }
                if(token.data == "while") {
                    token.type = TokenType_WHILE;
                    break;
                }
                if(token.data == "return") {
                    token.type = TokenType_RETURN;
                    break;
                }
                if(token.data == "new") {
    				token.type = TokenType_NEW;
    				break;
    			}
    			if(token.data == "delete") {
    				token.type = TokenType_DELETE;
    				break;
    			}
    			if(token.data == "null") {
    				token.type = TokenType_NULL;
    				break;
    			}
    
                if(token.data == "true" || token.data == "false") {
                    token.type = TokenType_BOOL;
                }
                break;
            }
            else if(IsNumeric(tokenizer.data[tokenizer.index])) {
                /* Features to add
                1) Checking for multiple . in float
                */
                token.type = TokenType_DIGIT;
                
                while(IsNumeric(tokenizer.data[tokenizer.index]) || tokenizer.data[tokenizer.index] == '_') {
                    if(tokenizer.data[tokenizer.index] == '_') {
                        ++tokenizer.index;
                        continue;
                    }
                    token.data.push_back(tokenizer.data[tokenizer.index]);
				    ++tokenizer.index;
				    if(tokenizer.index < tokenizer.data.size() && tokenizer.data[tokenizer.index] == '.') {
                        token.type = TokenType_FLOAT;
                        token.data.push_back(tokenizer.data[tokenizer.index]);
                        ++tokenizer.index;
				    }
			    }
                --tokenizer.index;
            }
        }
    }
    return token;
}

std::vector<Token> LexInput(const std::string input) {
    Tokenizer tokenizer;
    tokenizer.data = input;
    tokenizer.index = 0;

    std::vector<Token> token_array;
    while(1) {
        token_array.push_back(GetToken(tokenizer));
        ++tokenizer.index;
        if(tokenizer.index >= tokenizer.data.size() || token_array.back().type == TokenType_EOF) break;
    }
    
    return token_array;
}

std::string TokenTypeToString(const TokenType type) {
	switch (type) {
		case TokenType_IDENTIFIER: return "Identifier";
		case TokenType_DIGIT: return "Digit";
		case TokenType_FLOAT: return "Float";
		case TokenType_STRING: return "String";
		case TokenType_BOOL: return "Bool";
		case TokenType_PLUS: return "Plus";
		case TokenType_MINUS: return "Minus";
		case TokenType_TIMES: return "Times";
		case TokenType_SLASH: return "Slash";
		case TokenType_PERIOD: return "Period";
		case TokenType_EQL: return "Equal";
		case TokenType_NOT: return "Not";
		case TokenType_LSS: return "Less";
		case TokenType_GTR: return "Greater";
		case TokenType_LEQ: return "Less or equal";
		case TokenType_GEQ: return "Greater or equal";
		case TokenType_LPAREN: return "(";
		case TokenType_RPAREN: return ")";
		case TokenType_LBRACK: return "[";
		case TokenType_RBRACK: return "]";
		case TokenType_LBRACE: return "}";
		case TokenType_RBRACE: return "{";
		case TokenType_SEMICOLON: return ";";
		case TokenType_COMMA: return ",";
		case TokenType_BECOMES: return "Becomes";
		case TokenType_IF: return "If";
		case TokenType_WHILE: return "While";
		case TokenType_FOR: return "For";
		case TokenType_RETURN: return "Return";
		case TokenType_NEW: return "New";
		case TokenType_DELETE: return "Delete";
		case TokenType_NULL: return "Null";
		case TokenType_EOF: return "EndOfStream";
		case TokenType_UNKNOWN:
		default: return "Unknown";
	}
}

void DebugPrintTokenArray(const std::vector<Token> &token_array) {
    std::cout << "Token_array = {" << std::endl;
	for (Token it: token_array) {
        std::cout << TokenTypeToString(it.type) << " " << it.data << std::endl;
	}
    std::cout << "}" << std::endl;
}
