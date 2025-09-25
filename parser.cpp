#include "parser.hpp"

void ParseTree::make_tree()
{
    root = new ParseTree();
}

ParseTree *ParseTree::make_node(TokenType t, ParseTree *l, ParseTree *r)
{
    auto new_node = new ParseTree();
    new_node->token_type = t;
    new_node->content.operator_.left = l;
    new_node->content.operator_.right = r;
    return new_node;
}

ParseTree *ParseTree::make_node(TokenType t, ParseTree *c, double (*fp)(double))
{
    auto new_node = new ParseTree();
    new_node->token_type = t;
    new_node->content.function.child = c;
    new_node->content.function.fp = fp;
    return new_node;
}

ParseTree *ParseTree::make_node(TokenType t, double v)
{
    auto new_node = new ParseTree();
    new_node->token_type = t;
    if (t == TokenType::CONST_ID)
        new_node->content.r_value = v;
    else
        new_node->content.l_value = &T;
    return new_node;
}

void Parser::run()
{
    auto token = scanner.getToken();
    program(token);
}

void Parser::program(Token &tk)
{
    while (tk.type != TokenType::END)
    {
        statement(tk);
        if (!match_token(tk, TokenType::SEMICO))
            error(tk, "expect ';'");
    }
}

void Parser::statement(Token &tk)
{
    switch (tk.type)
    {
    case TokenType::FOR:
        for_statement(tk);
        break;
    case TokenType::ORIGIN:
        origin_statement(tk);
        break;
    case TokenType::SCALE:
        scale_statement(tk);
        break;
    case TokenType::ROTATE:
        rotate_statement(tk);
        break;
    default:
        error(tk, "invalid statement");
        break;
    }
}

void Parser::for_statement(Token &tk)
{
    if (!match_token(tk, TokenType::FOR))
        error(tk, "expect 'FOR'");
    auto token = tk;
}

void Parser::origin_statement(Token &tk) {}
void Parser::scale_statement(Token &tk) {}
void Parser::rotate_statement(Token &tk) {}

ParseTree *Parser::expression() {}
ParseTree *Parser::term() {}
ParseTree *Parser::factor() {}
ParseTree *Parser::component() {}
ParseTree *Parser::atom() {}