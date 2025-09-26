#ifndef PARSER_HPP
#define PARSER_HPP

#pragma once

#include "scanner.hpp"
#include <exception>
#include <string>

using std::string;

static double T;

class ParseTree
{
public:
    enum NodeType
    {
        CONST_NODE,
        VAR_NODE,
        UNARY_OP_NODE,
        BINARY_OP_NODE,
        FUNC_CALL_NODE
    } type;
    union
    {
        double const_val;
        struct
        {
            TokenType op;
            ParseTree *child;
        } unary_op;
        struct
        {
            TokenType op;
            ParseTree *left;
            ParseTree *right;
        } binary_op;
        struct
        {
            double (*func_ptr)(double);
            ParseTree *arg;
        } func_call;
    } data;

    // 构造函数
    ParseTree(double value) : type(CONST_NODE)
    {
        data.const_val = value;
    }

    ParseTree(NodeType node_type, TokenType op, ParseTree *child) : type(node_type)
    {
        if (node_type == UNARY_OP_NODE)
        {
            data.unary_op.op = op;
            data.unary_op.child = child;
        }
    }

    ParseTree(NodeType node_type, TokenType op, ParseTree *left, ParseTree *right) : type(node_type)
    {
        if (node_type == BINARY_OP_NODE)
        {
            data.binary_op.op = op;
            data.binary_op.left = left;
            data.binary_op.right = right;
        }
    }

    ParseTree(double (*func)(double), ParseTree *arg) : type(FUNC_CALL_NODE)
    {
        data.func_call.func_ptr = func;
        data.func_call.arg = arg;
    }

    // 变量节点构造函数
    ParseTree() : type(VAR_NODE) {}

    // 析构函数
    ~ParseTree()
    {
        // 根据节点类型释放子节点
        switch (type)
        {
        case UNARY_OP_NODE:
            delete data.unary_op.child;
            break;
        case BINARY_OP_NODE:
            delete data.binary_op.left;
            delete data.binary_op.right;
            break;
        case FUNC_CALL_NODE:
            delete data.func_call.arg;
            break;
        case CONST_NODE:
        case VAR_NODE:
            // 没有需要释放的子节点
            break;
        }
    }
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
    ParseTree *expression();
    ParseTree *term();
    ParseTree *factor();
    ParseTree *component();
    ParseTree *atom();
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