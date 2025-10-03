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
    enum NodeType
    {
        OP,
        FUNC,
        CONST,
        VAR,
    } type;
    union Content
    {
        struct op_node
        {
            TreeNode *left, *right;
        } operator_;
        struct func_node
        {
            TreeNode *child;
            double (*fp)(double);
        } function;
        double r_value;
        double *l_value;
    } content;

    TreeNode(TreeNode *l, TreeNode *r) : type(OP)
    {
        content.operator_.left = l;
        content.operator_.right = r;
    };
    TreeNode(double (*fp)(double)) : type(FUNC)
    {
        content.function.fp = fp;
    };
    TreeNode(double v) : type(CONST)
    {
        content.r_value = v;
    };
    TreeNode(double *p) : type(VAR)
    {
        content.l_value = p;
    };
    ~TreeNode()
    {
        if (type == OP)
        {
            delete content.operator_.left;
            delete content.operator_.right;
        }
        else if (type == FUNC)
        {
            delete content.function.child;
        }
    };
};

class Parser
{
    Scanner scanner;
    Token token;
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
    // error handling
    void error(const Token &tk, const string &msg = "")
    {
        throw std::runtime_error("Parser Error: " + msg + " at '" + tk.name + "'");
    };
    void error(const string &msg = "")
    {
        throw std::runtime_error("Parser Error: " + msg);
    };

public:
    Parser(string filename, int n = 4096) : scanner(filename, n) {};
    void run();
};

#endif