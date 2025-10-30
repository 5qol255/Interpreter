#ifndef PARSER_HPP
#define PARSER_HPP

#pragma once

#include "scanner.hpp"
#include <exception>
#include <string>
#include <vector>
#ifdef DEBUG
#include <iostream>
#endif

using std::string;

extern double T;

class TreeNode
{
public:
    enum class nodetypes
    {
        op,
        func,
        r_value,
        l_value
    } nodetype;
    union node_filling
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
        : left(left), right(right), nodetype(nodetypes::op) { filling.op = op; };
    TreeNode(double (*func)(double), TreeNode *left = nullptr, TreeNode *right = nullptr)
        : left(left), right(right), nodetype(nodetypes::func) { filling.func = func; };
    TreeNode(double r_value, TreeNode *left = nullptr, TreeNode *right = nullptr)
        : left(left), right(right), nodetype(nodetypes::r_value) { filling.r_value = r_value; };
    TreeNode(double *l_value, TreeNode *left = nullptr, TreeNode *right = nullptr)
        : left(left), right(right), nodetype(nodetypes::l_value) { filling.l_value = l_value; };
#ifdef DEBUG
    friend void travel(TreeNode *node);
#endif
};

class Parser
{
    Scanner scanner;
    std::vector<TreeNode *> parser_trees;
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
#ifdef DEBUG
    void print_trees() const
    {
        std::cout << parser_trees.size() << " parser_trees:\n";
        for (const auto &tree : parser_trees)
        {
            travel(tree);
            std::cout << '\n';
        }
    };
#endif
};

#endif