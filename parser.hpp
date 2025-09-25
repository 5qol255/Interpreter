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
    ParseTree *root;
    TokenType token_type;
    union
    {
        op_node operator_;
        func_node function;
        double r_value;
        double *l_value;
    } content;
    ParseTree() : root(nullptr) {};
    void make_tree();
    ParseTree *make_node(TokenType t, ParseTree *l, ParseTree *r);
    ParseTree *make_node(TokenType t, ParseTree *c, double (*fp)(double));
    ParseTree *make_node(TokenType t, double v);
};

class Parser
{
    Scanner scanner;
    // parsing functions
    void program(Token &);
    void statement(Token &);
    void for_statement(Token &);
    void origin_statement(Token &);
    void scale_statement(Token &);
    void rotate_statement(Token &);
    ParseTree *expression();
    ParseTree *term();
    ParseTree *factor();
    ParseTree *component();
    ParseTree *atom();
    // helper functions
    bool match_token(Token &tk, TokenType t) { return tk.type == t; }
    // error handling
    void error(Token &tk, string msg = "")
    {
        throw std::runtime_error("Parser Error: " + msg + " at '" + tk.name + "'");
    };

public:
    Parser(string filename, int n = 4096) : scanner(filename, n) {};
    void run();
};

#endif