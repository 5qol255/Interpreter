#include "parser.hpp"
#include <unordered_map>
#include <cmath>

using std::string;

// 变量T的定义
double T;

const string &type2str(TokenType t)
{
    // TokenType到字符串的映射
    static const std::unordered_map<TokenType, const std::string>
        tokentype_2_string = {
            {TokenType::ID, "ID"},           // never used
            {TokenType::COMMENT, "COMMENT"}, // never used
            {TokenType::CONST_ID, "CONST_ID"},
            {TokenType::FUNC, "FUNC"},
            {TokenType::T, "T"},
            /* 分隔符 */
            {TokenType::SEMICO, "SEMICO"},       // ;
            {TokenType::L_BRACKET, "L_BRACKET"}, // (
            {TokenType::R_BRACKET, "R_BRACKET"}, // )
            {TokenType::COMMA, "COMMA"},         // ,
            /* 运算符 */
            {TokenType::PLUS, "PLUS"},   // +
            {TokenType::MINUS, "MINUS"}, // -
            {TokenType::MUL, "MUL"},     // *
            {TokenType::DIV, "DIV"},     // /
            {TokenType::POWER, "POWER"}, // **
            /* 关键字 */
            {TokenType::ORIGIN, "ORIGIN"}, // 原点
            {TokenType::SCALE, "SCALE"},   // 缩放
            {TokenType::ROTATE, "ROTATE"}, // 旋转
            {TokenType::IS, "IS"},         // 是
            {TokenType::FROM, "FROM"},     // 从
            {TokenType::TO, "TO"},         // 到
            {TokenType::STEP, "STEP"},     // 步长
            {TokenType::FOR, "FOR"},       // 循环
            {TokenType::DRAW, "DRAW"},     // 绘制
            /* 其他 */
            {TokenType::END, "END"},    // 结束符
            {TokenType::ERROR, "ERROR"} // 非法记号
        };
    return tokentype_2_string.at(t);
}

void Parser::match_token(TokenType t)
{
    if (token.type != t)
        error(token, "unexpected token, expect '" + type2str(t) + "'");
    else
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
    match_token(TokenType::FOR);
    match_token(TokenType::T);
    match_token(TokenType::FROM);
    TreeNode *start_ptr = expression();
    match_token(TokenType::TO);
    TreeNode *end_ptr = expression();
    match_token(TokenType::STEP);
    TreeNode *step_ptr = expression();
    match_token(TokenType::DRAW);
    match_token(TokenType::L_BRACKET);
    TreeNode *x_ptr = expression();
    match_token(TokenType::COMMA);
    TreeNode *y_ptr = expression();
    match_token(TokenType::R_BRACKET);
    // delete start_ptr, end_ptr, step_ptr, x_ptr, y_ptr;
}

void Parser::origin_statement()
{
    match_token(TokenType::ORIGIN);
    match_token(TokenType::IS);
    match_token(TokenType::L_BRACKET);
    TreeNode *x_ptr = expression();
    match_token(TokenType::COMMA);
    TreeNode *y_ptr = expression();
    match_token(TokenType::R_BRACKET);
    // delete x_ptr, y_ptr;
}

void Parser::scale_statement()
{
    match_token(TokenType::SCALE);
    match_token(TokenType::IS);
    match_token(TokenType::L_BRACKET);
    TreeNode *x_ptr = expression();
    match_token(TokenType::COMMA);
    TreeNode *y_ptr = expression();
    match_token(TokenType::R_BRACKET);
    // delete x_ptr, y_ptr;
}

void Parser::rotate_statement()
{
    match_token(TokenType::ROTATE);
    match_token(TokenType::IS);
    TreeNode *angle_ptr = expression();
    // delete angle_ptr;
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
    switch (token.type)
    {
    case TokenType::CONST_ID:
    {
        match_token(TokenType::CONST_ID);
        return new TreeNode(token.value.v);
    }
    case TokenType::T:
    {
        match_token(TokenType::T);
        return new TreeNode(&T);
    }
    case TokenType::FUNC:
    {
        // 获取函数指针
        double (*func_ptr)(double) = token.value.func_ptr;
        match_token(TokenType::FUNC);
        match_token(TokenType::L_BRACKET);
        TreeNode *arg = expression();
        match_token(TokenType::R_BRACKET);
        TreeNode *node = new TreeNode(func_ptr);
        // 需要将参数设置为函数的子节点
        node->content.function.child = arg;
        return node;
    }
    case TokenType::L_BRACKET:
    {
        match_token(TokenType::L_BRACKET);
        TreeNode *node = expression();
        match_token(TokenType::R_BRACKET);
        return node;
    }
    default:
        error(token, "unexpected token in atom");
        return nullptr; // never reach here
    }
}