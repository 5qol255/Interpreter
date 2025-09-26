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

/************************************************************************/
ParseTree *Parser::expression()
{
    ParseTree *left = term();

    while (token.type == TokenType::PLUS || token.type == TokenType::MINUS)
    {
        TokenType op = token.type;
        match_token(token.type);
        ParseTree *right = term();
        left = new ParseTree(ParseTree::BINARY_OP_NODE, op, left, right);
    }

    return left;
}

ParseTree *Parser::term()
{
    ParseTree *left = factor();

    while (token.type == TokenType::MUL || token.type == TokenType::DIV)
    {
        TokenType op = token.type;
        match_token(token.type);
        ParseTree *right = factor();
        left = new ParseTree(ParseTree::BINARY_OP_NODE, op, left, right);
    }

    return left;
}

ParseTree *Parser::factor()
{
    if (token.type == TokenType::PLUS || token.type == TokenType::MINUS)
    {
        TokenType op = token.type;
        match_token(token.type);
        ParseTree *child = factor();
        return new ParseTree(ParseTree::UNARY_OP_NODE, op, child);
    }

    return component();
}

ParseTree *Parser::component()
{
    ParseTree *left = atom();

    if (token.type == TokenType::POWER)
    {
        match_token(TokenType::POWER);
        ParseTree *right = component();
        return new ParseTree(ParseTree::BINARY_OP_NODE, TokenType::POWER, left, right);
    }

    return left;
}

ParseTree *Parser::atom()
{
    ParseTree *node = nullptr;

    switch (token.type)
    {
    case TokenType::CONST_ID:
        node = new ParseTree(token.value.v);
        match_token(TokenType::CONST_ID);
        break;

    case TokenType::T:
        node = new ParseTree(); // 创建变量节点
        match_token(TokenType::T);
        break;

    case TokenType::FUNC:
        // 这里需要根据函数名获取函数指针
        // 假设token.value.func_ptr已经正确设置
        {
            double (*func_ptr)(double) = token.value.func_ptr;
            match_token(TokenType::FUNC);
            match_token(TokenType::L_BRACKET);
            ParseTree *arg = expression();
            match_token(TokenType::R_BRACKET);
            node = new ParseTree(func_ptr, arg);
        }
        break;

    case TokenType::L_BRACKET:
        match_token(TokenType::L_BRACKET);
        node = expression();
        match_token(TokenType::R_BRACKET);
        break;

    default:
        error(token, "invalid atom");
        break;
    }

    return node;
}