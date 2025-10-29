#ifndef PARSER_HPP
#define PARSER_HPP

#pragma once

#include "scanner.hpp"
#include <exception>
#include <string>

using std::string;

extern double T;

class TreeNode
{
public:
    union a
    {
        TokenType op;           // 运算符
        double (*func)(double); // 函数指针
        double r_value;         // 数值(右)
        double *l_value;        // 数值(左)
    } filling;
    TreeNode *left, *right;
    // 四种不同节点对应的构造函数
    TreeNode(TreeNode *left = nullptr, TreeNode *right = nullptr)
        : left(left), right(right) {};
    TreeNode(TokenType op, TreeNode *left = nullptr, TreeNode *right = nullptr)
        : left(left), right(right) { filling.op = op; };
    TreeNode(double (*func)(double), TreeNode *left = nullptr, TreeNode *right = nullptr)
        : left(left), right(right) { filling.func = func; };
    TreeNode(double r_value, TreeNode *left = nullptr, TreeNode *right = nullptr)
        : left(left), right(right) { filling.r_value = r_value; };
    TreeNode(double *l_value, TreeNode *left = nullptr, TreeNode *right = nullptr)
        : left(left), right(right) { filling.l_value = l_value; };
};

class Parser
{
    Scanner scanner;
    /* 函数声明 */
    // parsing functions
    void program();
    void statement();
    void for_statement();
    void origin_statement();
    void scale_statement();
    void rotate_statement();
    TreeNode *expression();
    TreeNode *term();
    TreeNode *factor();
    TreeNode *component();
    TreeNode *atom();
    // helper functions
    void match_token(TokenType t);
    static void error(const Token &tk = Token(), const string &msg = "");

public:
    Parser(const string &filename, int n = 4096) : scanner(filename, n) {};
    void run() { program(); };
};

#endif