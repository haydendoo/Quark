#include "parser.hpp"
#include "lexer.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

template<typename T>
std::vector<T> slice(std::vector<T> &v, int m, int n) {
    std::vector<T> vec(n - m + 1);
    std::copy(v.begin() + m, v.begin() + n + 1, vec.begin());
    return vec;
}

BinNode* parse(const std::vector<Token> &token_array) {
    BinNode *root = new BinNode(std::stof(token_array[0].data), TokenType_FLOAT);
    root->left = new BinNode(0, TokenType_FLOAT);
    root->right = new BinNode(std::stof(token_array[0].data), TokenType_FLOAT);
    root->op = TokenType_PLUS;
    for(int i = 1; i < token_array.size(); ++i) {
        if(token_array[i].type == TokenType_PLUS || token_array[i].type == TokenType_MINUS) {
            ++i;
            BinNode *temp = new BinNode();
            temp->left = root;
            temp->right = new BinNode(std::stof(token_array[i].data), TokenType_FLOAT);
            temp->op = token_array[i-1].type;
            if(temp->op == TokenType_PLUS) temp->ans = temp->left->ans + temp->right->ans;
            else temp->ans = temp->left->ans - temp->right->ans;
            root = temp;
            continue;
        }
        if(token_array[i].type == TokenType_TIMES || token_array[i].type == TokenType_SLASH) {
            ++i;
            BinNode *temp = root->right;
            temp->left = new BinNode(temp->ans, temp->op);
            temp->right = new BinNode(std::stof(token_array[i].data), TokenType_FLOAT);
            temp->op = token_array[i-1].type;
            if(temp->op == TokenType_TIMES) temp->ans = temp->left->ans * temp->right->ans;
            else temp->ans = temp->left->ans / temp->right->ans;
            
            if(root->op == TokenType_PLUS) root->ans = root->left->ans + root->right->ans;
            else if(root->op == TokenType_MINUS) root->ans = root->left->ans - root->right->ans;
            else if(root->op == TokenType_TIMES) root->ans = root->left->ans * root->right->ans;
            else root->ans = root->left->ans / root->right->ans;
        }
    }
    return root;
}

void DeleteTree(BinNode *root) {
    if(!root) return;
    DeleteTree(root->left);
    DeleteTree(root->right);
    delete root;
}

double _eval(std::vector<Token> &token_array, const std::unordered_map<int, int> &close) {
    std::vector<Token> ans;
    for(int i = 0; i < token_array.size(); ++i) {
        if(token_array[i].type == TokenType_LPAREN) {
            Token temp;
            std::vector<Token> temp2 = slice(token_array, i+1, close.at(i)-1);
            temp.type = TokenType_FLOAT;
            temp.data = std::to_string(_eval(temp2, close));
            ans.push_back(temp);
            i = close.at(i);
            continue;
        }
        if(token_array[i].type == TokenType_FLOAT || token_array[i].type == TokenType_DIGIT || token_array[i].type == TokenType_PLUS || token_array[i].type == TokenType_MINUS || token_array[i].type == TokenType_TIMES || token_array[i].type == TokenType_SLASH) ans.push_back(token_array[i]);
    }
    return parse(ans)->ans;
}

double eval(std::vector<Token> &token_array) {
    std::vector<int> stk;
    std::unordered_map<int, int> close;
    for(int i = 0; i < token_array.size(); ++i) {
        if(token_array[i].type == TokenType_LPAREN) {
            stk.push_back(i);
        }
        else if(token_array[i].type == TokenType_RPAREN) {
            if(stk.empty()) {
                std::cout << "Brackets are invalid" << std::endl;
                return 0;
            }
            close[stk.back()] = i;
            stk.pop_back();
        }
    }
    if(!stk.empty()) {
        std::cout << "Brackets are invalid" << std::endl;
        return 0;
    }
    for(auto it: close) {
        std::cout << it.first << ' ' << it.second << std::endl;
    }
    return _eval(token_array, close);
}
