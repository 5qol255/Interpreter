#ifndef PARSER_HPP
#define PARSER_HPP

#pragma once

#include "scanner.hpp"

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

private:
    ParseTree *root;
    TokenType token_type;
    union
    {
        op_node operator_;
        func_node function;
        double r_value;
        double *l_value;
    } content;

public:
    ParseTree() : root(nullptr) {};
    void make_tree();
    void make_node(TokenType t, ParseTree *l, ParseTree *r);
    void make_node(TokenType t, ParseTree *c, double (*fp)(double));
    void make_node(TokenType t, double v);
    void make_node(TokenType t, double *v);
};

#endif