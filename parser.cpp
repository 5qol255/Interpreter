#include "parser.hpp"

void Parser::match_token(TokenType t)
{
    if (token.type != t)
        error(token, "unexpected token, expect '" + std::to_string(int(t)) + "'");
    token = scanner.getToken();
}

void Parser::run()
{
    token = scanner.getToken();
    program();
}

void Parser::program()
{
    while (token.type != TokenType::END)
    {
        statement();
        match_token(TokenType::SEMICO);
    }
}

void Parser::statement()
{
    switch (token.type)
    {
    case TokenType::FOR:
        for_statement();
        break;
    case TokenType::ORIGIN:
        origin_statement();
        break;
    case TokenType::SCALE:
        scale_statement();
        break;
    case TokenType::ROTATE:
        rotate_statement();
        break;
    default:
        error(token, "invalid statement");
        break;
    }
}

void Parser::for_statement()
{
    ParseTree *start_ptr, *end_ptr, *step_ptr, *x_ptr, *y_ptr;
    match_token(TokenType::FOR);
    match_token(TokenType::T);
    match_token(TokenType::FROM);
    start_ptr = expression();
    match_token(TokenType::TO);
    end_ptr = expression();
    match_token(TokenType::STEP);
    step_ptr = expression();
    match_token(TokenType::DRAW);
    match_token(TokenType::L_BRACKET);
    x_ptr = expression();
    match_token(TokenType::COMMA);
    y_ptr = expression();
    match_token(TokenType::R_BRACKET);
}

void Parser::origin_statement()
{
    ParseTree *x_ptr, *y_ptr;
    match_token(TokenType::ORIGIN);
    match_token(TokenType::IS);
    match_token(TokenType::L_BRACKET);
    x_ptr = expression();
    match_token(TokenType::COMMA);
    y_ptr = expression();
    match_token(TokenType::R_BRACKET);
}

void Parser::scale_statement()
{
    ParseTree *x_ptr, *y_ptr;
    match_token(TokenType::SCALE);
    match_token(TokenType::IS);
    match_token(TokenType::L_BRACKET);
    x_ptr = expression();
    match_token(TokenType::COMMA);
    y_ptr = expression();
    match_token(TokenType::R_BRACKET);
}

void Parser::rotate_statement()
{
    ParseTree *angle_ptr;
    match_token(TokenType::ROTATE);
    match_token(TokenType::IS);
    angle_ptr = expression();
}

ParseTree *Parser::expression()
{
    ParseTree *left, *right;
    TokenType op;
    left = term();
    while (token.type == TokenType::PLUS || token.type == TokenType::MINUS)
    {
        op = token.type;
        match_token(op);
        right = term();
        left = left->make_node(op, left, right);
    }
    return left;
}

ParseTree *Parser::term()
{
    ParseTree *left, *right;
    TokenType op;
    left = factor();
    while (token.type == TokenType::MUL || token.type == TokenType::DIV)
    {
        op = token.type;
        match_token(op);
        right = factor();
        left = left->make_node(op, left, right);
    }
    return left;
}

ParseTree *Parser::factor()
{
}

ParseTree *Parser::component()
{
}

ParseTree *Parser::atom()
{
    switch (token.type)
    {
    case TokenType::CONST_ID:
        return ParseTree().make_node(TokenType::CONST_ID, token.value.v);
        break;

    default:
        break;
    }
}