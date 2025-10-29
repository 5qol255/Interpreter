#include "parser.hpp"
#include <unordered_map>
#include <cmath>

using std::string;

/* 变量T的定义 */
double T;
/* 辅助变量 */
Token token;
const std::unordered_map<TokenType, string> tokentype2str = {
    {TokenType::PLUS, "PLUS"},
    {TokenType::MINUS, "MINUS"},
    {TokenType::MUL, "MUL"},
    {TokenType::DIV, "DIV"},
    {TokenType::POWER, "POWER"},
    {TokenType::L_BRACKET, "L_BRACKET"},
    {TokenType::R_BRACKET, "R_BRACKET"},
    {TokenType::COMMA, "COMMA"},
    {TokenType::SEMICO, "SEMICO"},
    {TokenType::ID, "ID"},
    {TokenType::END, "END"},
    {TokenType::ERROR, "ERROR"},
    {TokenType::T, "T"},
    {TokenType::ORIGIN, "ORIGIN"},
    {TokenType::SCALE, "SCALE"},
    {TokenType::ROTATE, "ROTATE"},
    {TokenType::IS, "IS"},
    {TokenType::FROM, "FROM"},
    {TokenType::TO, "TO"},
    {TokenType::STEP, "STEP"},
    {TokenType::FOR, "FOR"},
    {TokenType::DRAW, "DRAW"},
    {TokenType::CONST_ID, "CONST_ID"},
    {TokenType::FUNC, "FUNCTION"},
};

void Parser::error(const Token &tk, const string &msg)
{
    static string message;
    message = "Parser Error: " + msg + "\nprovided token: " + tk.name + " (" + tokentype2str.at(tk.type) + ")\n";
    throw std::runtime_error(message);
}

void Parser::match_token(TokenType t)
{
    if (token.type == t)
        token = scanner.getToken();
    else
        Parser::error(token, "expect " + tokentype2str.at(t));
}

void Parser::program()
{
    token = scanner.getToken(); // 初始化token
    while (token.type != TokenType::END)
    {
        statement();
        match_token(TokenType::SEMICO);
    }
}

void Parser::statement()
{
    if (token.type == TokenType::FOR)
        for_statement();
    else if (token.type == TokenType::ORIGIN)
        origin_statement();
    else if (token.type == TokenType::SCALE)
        scale_statement();
    else if (token.type == TokenType::ROTATE)
        rotate_statement();
    else
        Parser::error(token, "invalid statement");
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
    TreeNode *left, *right;
    left = term();
    while (token.type == TokenType::PLUS || token.type == TokenType::MINUS)
    {
        match_token(token.type);
        right = term();
        left = new TreeNode(token.type, left, right);
    }
    return left;
}

TreeNode *Parser::term()
{
    TreeNode *left, *right;
    left = factor();
    while (token.type == TokenType::MUL || token.type == TokenType::DIV)
    {
        match_token(token.type);
        right = factor();
        left = new TreeNode(token.type, left, right);
    }
    return left;
}

TreeNode *Parser::factor()
{
    TreeNode *node;
    if (token.type == TokenType::PLUS || token.type == TokenType::MINUS)
    {
        auto op = token.type;
        match_token(token.type);
        if (op == TokenType::MINUS)
        {
            auto zero_node = new TreeNode(0.0);
            node = new TreeNode(token.type, zero_node, component());
        }
        else
            node = component();
    }
    else
        node = component();
    return node;
}

TreeNode *Parser::component()
{
    TreeNode *left, *right;
    left = atom();
    if (token.type == TokenType::POWER)
    {
        auto op = token.type;
        match_token(TokenType::POWER);
        right = component();
        left = new TreeNode(TokenType::POWER, left, right);
    }
    return left;
}

TreeNode *Parser::atom()
{
    TreeNode *node;
    if (token.type == TokenType::CONST_ID)
    {
        node = new TreeNode(token.value.v);
        match_token(TokenType::CONST_ID);
    }
    else if (token.type == TokenType::T)
    {
        node = new TreeNode(&T);
        match_token(TokenType::T);
    }
    else if (token.type == TokenType::FUNC)
    {
        node = new TreeNode(token.value.func_ptr);
        match_token(TokenType::FUNC);
        match_token(TokenType::L_BRACKET);
        node->right = expression(); // 函数的参数挂在右节点
        match_token(TokenType::R_BRACKET);
        return node;
    }
    else if (token.type == TokenType::L_BRACKET)
    {
        match_token(TokenType::L_BRACKET);
        node = expression();
        match_token(TokenType::R_BRACKET);
    }
    else
        error(token, "invalid atom");
    return node;
}