#include "parser.hpp"
#include <stdexcept>
#include <unordered_map>
#include <stack>
#include <cmath>
#ifdef DEBUG
#include <iostream>
#endif

using std::string;

/* 定义Parser类的静态成员变量 */
std::vector<Point> Parser::point_list;

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
    {TokenType::Error, "Error"},
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
std::vector<TreeNode *> node_sequence;
#if DEBUG >= 1
const std::unordered_map<double (*)(double), string> func_table = {
    {std::sin, "SIN"},
    {std::cos, "COS"},
    {std::tan, "TAN"},
    {std::log, "LN"},
    {std::exp, "EXP"},
    {std::sqrt, "SQRT"},
}; // 反向函数表，方便打印函数名
#endif

/* 辅助函数 */
// 后续遍历,为了方便求表达式的值
void post_order_travel(TreeNode *node, std::vector<TreeNode *> &post_order_list)
{
    if (node == nullptr)
        return;
    post_order_travel(node->left, post_order_list);
    post_order_travel(node->right, post_order_list);
    post_order_list.emplace_back(node);
}

// 求表达式的值
double evaluate(TreeNode *node, std::vector<TreeNode *> &post_order_list)
{
    // 先清零
    post_order_list.clear();
    // 再求后序遍历序列
    post_order_travel(node, post_order_list);
    // 这里判断一下后序遍历序列是否为空,为空抛出一个异常
    if (post_order_list.empty())
        throw std::runtime_error("empty post_order_list");
    // 逆序求值
    std::stack<double> value_stack;
    for (auto const &node : post_order_list)
    {
        if (node->nodetype == TreeNode::nodetypes::r_value)
            // 常数直接压栈
            value_stack.push(node->filling.r_value);
        else if (node->nodetype == TreeNode::nodetypes::func)
        {
            // 弹出栈顶数值施加函数后压栈
            double arg = value_stack.top();
            value_stack.pop();
            value_stack.push(node->filling.func(arg));
        }
        else if (node->nodetype == TreeNode::nodetypes::op)
        {
            // 弹出两个数值,计算结果,再压栈
            double right = value_stack.top();
            value_stack.pop();
            double left = value_stack.top();
            value_stack.pop();
            switch (node->filling.op)
            {
            case TokenType::PLUS:
                value_stack.push(left + right);
                break;
            case TokenType::MINUS:
                value_stack.push(left - right);
                break;
            case TokenType::MUL:
                value_stack.push(left * right);
                break;
            case TokenType::DIV:
                value_stack.push(left / right);
                break;
            case TokenType::POWER:
                value_stack.push(std::pow(left, right));
                break;
            }
        }
        else
            // 把变量T的值压栈
            value_stack.push(*(node->filling.l_value));
    }
    // 栈顶元素就是表达式的值
    return value_stack.top();
}

#if DEBUG >= 1
// 中序遍历,为了方便打印表达式
void travel(TreeNode *node)
{
    if (node == nullptr)
        return;
    // 遍历左子树
    if (node->left != nullptr)
        travel(node->left);
    else
        std::cout << "null ";
    // 访问根节点
    if (node->nodetype == TreeNode::nodetypes::op)
        std::cout << tokentype2str.at(node->filling.op);
    else if (node->nodetype == TreeNode::nodetypes::func)
        std::cout << func_table.at(node->filling.func);
    else if (node->nodetype == TreeNode::nodetypes::r_value)
        std::cout << node->filling.r_value;
    else if (node->nodetype == TreeNode::nodetypes::l_value)
        std::cout << *(node->filling.l_value);
    std::cout << ' ';
    // 遍历右子树
    if (node->right != nullptr)
        travel(node->right);
    else
        std::cout << "null ";
};
#endif

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

void Parser::match_token(enum class TokenType t)
{
    if (token.type == t)
    {
        token = scanner.getToken();
#if DEBUG >= 2
        std::cout << "matched token: " << tokentype2str.at(token.type) << std::endl;
#endif
    }
    else
        Parser::error(token, "expect " + tokentype2str.at(t));
}

void Parser::program()
{
#if DEBUG >= 2
    std::cout << "enter program\n";
#endif
    token = scanner.getToken(); // 初始化token
#if DEBUG >= 2
    std::cout << "matched token: " << tokentype2str.at(token.type) << std::endl;
#endif
    while (token.type != TokenType::END)
    {
        statement();
        match_token(TokenType::SEMICO);
    }
#if DEBUG >= 2
    std::cout << "exit program\n";
#endif
}

void Parser::statement()
{
#if DEBUG >= 2
    std::cout << "enter statement\n";
#endif
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
#if DEBUG >= 2
    std::cout << "exit statement\n";
#endif
}

void Parser::for_statement()
{
#if DEBUG >= 2
    std::cout << "enter for_statement\n";
#endif
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
    // 计算T的range和step
    from_ = evaluate(start_ptr, node_sequence);
    to_ = evaluate(end_ptr, node_sequence);
    step_ = evaluate(step_ptr, node_sequence);

    while (next_T() != nullptr)
    {
        // 每次循环重新计算可能含T的表达式的值
        double x = evaluate(x_ptr, node_sequence);
        double y = evaluate(y_ptr, node_sequence);
        double temp_x, temp_y;
        // 先放缩
        x *= scale_x;
        y *= scale_y;
        // 再旋转（逆时针）
        temp_x = x * std::cos(rotate_angle) - y * std::sin(rotate_angle);
        temp_y = x * std::sin(rotate_angle) + y * std::cos(rotate_angle);
        // 最后平移
        x = temp_x + origin_x;
        y = temp_y + origin_y;
        // 创建点的对象存入待画列表
        point_list.emplace_back(Point(x, y));
    }
#if DEBUG >= 1
    // 打印点列表
    for (auto const &point : point_list)
        std::cout << point.x << ',' << point.y << '\n';
#endif
    // 每次for-draw语句执行完毕后都要参数复位
    reset_args();
#if DEBUG >= 2
    std::cout << "exit for_statement\n";
#endif
#if DEBUG >= 1
    travel(start_ptr);
    std::cout << '\n';
    travel(end_ptr);
    std::cout << '\n';
    travel(step_ptr);
    std::cout << '\n';
    travel(x_ptr);
    std::cout << '\n';
    travel(y_ptr);
    std::cout << "\n";
#endif
}

void Parser::origin_statement()
{
#if DEBUG >= 2
    std::cout << "enter origin_statement\n";
#endif
    match_token(TokenType::ORIGIN);
    match_token(TokenType::IS);
    match_token(TokenType::L_BRACKET);
    TreeNode *x_ptr = expression();
    match_token(TokenType::COMMA);
    TreeNode *y_ptr = expression();
    match_token(TokenType::R_BRACKET);
    // 调用求值函数对表达式进行求值
    origin_x = evaluate(x_ptr, node_sequence);
    origin_y = evaluate(y_ptr, node_sequence);
#if DEBUG >= 2
    std::cout << "exit origin_statement\n";
#endif
#if DEBUG >= 1
    travel(x_ptr);
    std::cout << '\n';
    travel(y_ptr);
    std::cout << "\n";
#endif
}

void Parser::scale_statement()
{
#if DEBUG >= 2
    std::cout << "enter scale_statement\n";
#endif
    match_token(TokenType::SCALE);
    match_token(TokenType::IS);
    match_token(TokenType::L_BRACKET);
    TreeNode *x_ptr = expression();
    match_token(TokenType::COMMA);
    TreeNode *y_ptr = expression();
    match_token(TokenType::R_BRACKET);
    // 调用求值函数对表达式进行求值
    scale_x = evaluate(x_ptr, node_sequence);
    scale_y = evaluate(y_ptr, node_sequence);
#if DEBUG >= 2
    std::cout << "exit scale_statement\n";
#endif
#if DEBUG >= 1
    travel(x_ptr);
    std::cout << '\n';
    travel(y_ptr);
    std::cout << "\n";
#endif
}

void Parser::rotate_statement()
{
#if DEBUG >= 2
    std::cout << "enter rotate_statement\n";
#endif
    match_token(TokenType::ROTATE);
    match_token(TokenType::IS);
    TreeNode *angle_ptr = expression();
    // 调用求值函数对表达式进行求值
    rotate_angle = evaluate(angle_ptr, node_sequence);
#if DEBUG >= 2
    std::cout << "exit rotate_statement\n";
#endif
#if DEBUG >= 1
    travel(angle_ptr);
    std::cout << "\n";
#endif
}

TreeNode *Parser::expression()
{
#if DEBUG >= 2
    std::cout << "enter expression\n";
#endif
    TreeNode *left, *right;
    left = term();
    while (token.type == TokenType::PLUS || token.type == TokenType::MINUS)
    {
        auto op = token.type;
        match_token(token.type);
        right = term();
        left = new TreeNode(op, left, right);
    }
#if DEBUG >= 2
    std::cout << "exit expression\n";
#endif
    return left;
}

TreeNode *Parser::term()
{
#if DEBUG >= 2
    std::cout << "enter term\n";
#endif
    TreeNode *left, *right;
    left = factor();
    while (token.type == TokenType::MUL || token.type == TokenType::DIV)
    {
        auto op = token.type;
        match_token(token.type);
        right = factor();
        left = new TreeNode(op, left, right);
    }
#if DEBUG >= 2
    std::cout << "exit term\n";
#endif
    return left;
}

TreeNode *Parser::factor()
{
#if DEBUG >= 2
    std::cout << "enter factor\n";
#endif
    TreeNode *node;
    if (token.type == TokenType::PLUS || token.type == TokenType::MINUS)
    {
        auto op = token.type;
        match_token(token.type);
        if (op == TokenType::MINUS)
        {
            auto zero_node = new TreeNode(0.0);
            node = new TreeNode(op, zero_node, component());
        }
        else
            node = component();
    }
    else
        node = component();
#if DEBUG >= 2
    std::cout << "exit factor\n";
#endif
    return node;
}

TreeNode *Parser::component()
{
#if DEBUG >= 2
    std::cout << "enter component\n";
#endif
    TreeNode *left, *right;
    left = atom();
    while (token.type == TokenType::POWER)
    {
        match_token(TokenType::POWER);
        right = atom();
        left = new TreeNode(TokenType::POWER, left, right);
    }
#if DEBUG >= 2
    std::cout << "exit component\n";
#endif
    return left;
}

TreeNode *Parser::atom()
{
#if DEBUG >= 2
    std::cout << "enter atom\n";
#endif
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
        node = expression();
        match_token(TokenType::R_BRACKET);
    }
    else
        error(token, "invalid atom");
#if DEBUG >= 2
    std::cout << "exit atom\n";
#endif
    return node;
}

// 复位参数为默认值
void Parser::reset_args()
{
    origin_x = 0.0;
    origin_y = 0.0;
    scale_x = 1.0;
    scale_y = 1.0;
    rotate_angle = 0.0;
}

// 获取下一个T的值,返回指向T的指针,若超出范围则返回nullptr
double *Parser::next_T()
{
    if (!looping)
    {
        T = from_;
        looping = true;
        return &T;
    }
    else if (T + step_ < to_)
    {
        T += step_;
        return &T;
    }
    else
    {
        looping = false;
        return nullptr;
    }
}