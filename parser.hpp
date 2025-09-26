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
    struct op_node
    {
        ParseTree *left, *right;
    };
    struct func_node
    {
        ParseTree *child;
        double (*fp)(double);
    };

public:
    TokenType token_type;
    union
    {
        op_node operator_;
        func_node function;
        double r_value;
        double *l_value;
    } content;
    ParseTree(TokenType t) : token_type(t) {};
    ParseTree(TokenType t, ParseTree *l, ParseTree *r);
    ParseTree(TokenType t, ParseTree *c, double (*fp)(double));
    ParseTree(TokenType t, double v);
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