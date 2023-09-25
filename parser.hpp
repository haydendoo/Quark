#pragma once

#include "lexer.hpp"

#include <iostream>
#include <vector>
#include <string>

struct BinNode {
    double ans;
    TokenType op;
    BinNode *left, *right;
    BinNode(): ans(0.0), op(TokenType_FLOAT), left(NULL), right(NULL) {}
    BinNode(double p_ans, TokenType p_op): ans(p_ans), op(p_op), left(NULL), right(NULL) {}
};

BinNode* parse(const std::vector<Token> &token_array);
void DeleteTree(BinNode *root);
double eval(std::vector<Token> &token_array);
