#include "parser.hpp"
#include <unordered_map>
#include <cmath>

#ifdef DEBUG
#include <iostream>
#endif

using std::string;

/* 变量T的定义 */
double T = 0.0;
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
    message.clear();
    message += "\nParser Error: ";
    message += msg;
    message += "\nprovided token: ";
    message += tk.name;
    message += " (";
    message += tokentype2str.at(tk.type);
    message += ')';
    throw std::runtime_error(message);
}

void Parser::match_token(TokenType t)
{
    // #ifdef DEBUG
    //     printf("%s %s\n", token.name.c_str(), tokentype2str.at(t).c_str());
    // #endif
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
    match_token(TokenType::FOR);
    match_token(TokenType::T);
    match_token(TokenType::FROM);
    TreeNode *start_ptr = expression();
    parser_trees.push_back(start_ptr);
    match_token(TokenType::TO);
    TreeNode *end_ptr = expression();
    parser_trees.push_back(end_ptr);
    match_token(TokenType::STEP);
    TreeNode *step_ptr = expression();
    parser_trees.push_back(step_ptr);
    match_token(TokenType::DRAW);
    match_token(TokenType::L_BRACKET);
    TreeNode *x_ptr = expression();
    parser_trees.push_back(x_ptr);
    match_token(TokenType::COMMA);
    TreeNode *y_ptr = expression();
    parser_trees.push_back(y_ptr);
    match_token(TokenType::R_BRACKET);
}

void Parser::origin_statement()
{
    match_token(TokenType::ORIGIN);
    match_token(TokenType::IS);
    match_token(TokenType::L_BRACKET);
    TreeNode *x_ptr = expression();
    parser_trees.push_back(x_ptr);
    match_token(TokenType::COMMA);
    TreeNode *y_ptr = expression();
    parser_trees.push_back(y_ptr);
    match_token(TokenType::R_BRACKET);
}

void Parser::scale_statement()
{
    match_token(TokenType::SCALE);
    match_token(TokenType::IS);
    match_token(TokenType::L_BRACKET);
    TreeNode *x_ptr = expression();
    parser_trees.push_back(x_ptr);
    match_token(TokenType::COMMA);
    TreeNode *y_ptr = expression();
    parser_trees.push_back(y_ptr);
    match_token(TokenType::R_BRACKET);
}

void Parser::rotate_statement()
{
    match_token(TokenType::ROTATE);
    match_token(TokenType::IS);
    TreeNode *angle_ptr = expression();
    parser_trees.push_back(angle_ptr);
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
    }
    else if (token.type == TokenType::L_BRACKET)
    {
        match_token(TokenType::L_BRACKET);
        TreeNode *node = expression();
        match_token(TokenType::R_BRACKET);
    }
    else
        error(token, "invalid atom");
    return node;
}

#ifdef DEBUG
const std::unordered_map<double (*)(double), string> func_table = {
    {std::sin, "SIN"},
    {std::cos, "COS"},
    {std::tan, "TAN"},
    {std::log, "LN"},
    {std::exp, "EXP"},
    {std::sqrt, "SQRT"},
}; // 函数表
void travel(TreeNode *node)
{
    if (node == nullptr)
        return;
    if (node->left != nullptr)
        travel(node->left);

    if (node->nodetype == TreeNode::nodetypes::op)
        std::cout << tokentype2str.at(node->filling.op);
    else if (node->nodetype == TreeNode::nodetypes::func)
        std::cout << func_table.at(node->filling.func);
    else if (node->nodetype == TreeNode::nodetypes::r_value)
        std::cout << node->filling.r_value;
    else if (node->nodetype == TreeNode::nodetypes::l_value)
        std::cout << *(node->filling.l_value);

    if (node->right != nullptr)
        travel(node->right);
    std::cout << ' ';
};
#endif