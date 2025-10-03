#include "parser.hpp"
#include <unordered_map>
#include <cmath>

using std::string;

// 变量T的定义
double T;
// 函数表
const std::unordered_map<string, double (*)(double)> func_table = {
    {"SIN", sin},
    {"COS", cos},
    {"TAN", tan},
    {"LN", log},
    {"EXP", exp},
    {"SQRT", sqrt},
};

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
    TreeNode *start_ptr, *end_ptr, *step_ptr, *x_ptr, *y_ptr;
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
    TreeNode *x_ptr, *y_ptr;
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
    TreeNode *x_ptr, *y_ptr;
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
    TreeNode *angle_ptr;
    match_token(TokenType::ROTATE);
    match_token(TokenType::IS);
    angle_ptr = expression();
}

TreeNode *Parser::expression()
{
    TreeNode *left = term();
    while (token.type == TokenType::PLUS || token.type == TokenType::MINUS)
    {
        TokenType op = token.type;
        match_token(op);
        TreeNode *right = term();
        left = new TreeNode(left, right);
    }
    return left;
}

TreeNode *Parser::term()
{
    TreeNode *left = factor();
    while (token.type == TokenType::MUL || token.type == TokenType::DIV)
    {
        TokenType op = token.type;
        match_token(op);
        TreeNode *right = factor();
        left = new TreeNode(left, right);
    }
    return left;
}

TreeNode *Parser::factor()
{
    if (token.type == TokenType::PLUS || token.type == TokenType::MINUS)
    {
        TokenType op = token.type;
        match_token(op);
        TreeNode *child = factor();
        TreeNode *zero = new TreeNode(0.0);
        return new TreeNode(zero, child);
    }
    else
        return component();
}

TreeNode *Parser::component()
{
    TreeNode *base = atom();
    if (token.type == TokenType::POWER)
    {
        match_token(TokenType::POWER);
        TreeNode *exponent = component();
        return new TreeNode(base, exponent);
    }
    else
        return base;
}

TreeNode *Parser::atom()
{
    TreeNode *node = nullptr;
    switch (token.type)
    {
    case TokenType::CONST_ID:
    {
        node = new TreeNode(token.value.v);
        match_token(TokenType::CONST_ID);
        break;
    }
    case TokenType::T:
    {
        node = new TreeNode(&T); // T是全局变量
        match_token(TokenType::T);
        break;
    }
    case TokenType::FUNC:
    {
        // 获取函数指针
        double (*func_ptr)(double) = token.value.func_ptr;
        match_token(TokenType::FUNC);
        match_token(TokenType::L_BRACKET);
        TreeNode *arg = expression();
        match_token(TokenType::R_BRACKET);
        node = new TreeNode(func_ptr);
        // 需要将参数设置为函数的子节点
        node->content.function.child = arg;
        break;
    }
    case TokenType::L_BRACKET:
    {
        match_token(TokenType::L_BRACKET);
        node = expression();
        match_token(TokenType::R_BRACKET);
        break;
    }
    default:
    {
        error(token, "unexpected token in atom");
        break;
    }
    }
    return node;
}