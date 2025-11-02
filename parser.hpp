#ifndef PARSER_HPP
#define PARSER_HPP

#pragma once

#include "scanner.hpp"
#include <string>
#include <vector>
#include <tuple>

using std::string;

class TreeNode
{
public:
    enum class nodetypes
    {
        op,
        func,
        r_value,
        l_value
    } nodetype;
    union node_filling
    {
        enum class TokenType op; // 运算符
        double (*func)(double);  // 函数指针
        double r_value;          // 数值(右)
        double *l_value;         // 数值(左)
    } filling;
    TreeNode *left, *right;
    // 四种不同节点对应的构造函数
    TreeNode(TreeNode *left = nullptr, TreeNode *right = nullptr)
        : left(left), right(right) {};
    TreeNode(enum class TokenType op, TreeNode *left = nullptr, TreeNode *right = nullptr)
        : left(left), right(right), nodetype(nodetypes::op) { filling.op = op; };
    TreeNode(double (*func)(double), TreeNode *left = nullptr, TreeNode *right = nullptr)
        : left(left), right(right), nodetype(nodetypes::func) { filling.func = func; };
    TreeNode(double r_value, TreeNode *left = nullptr, TreeNode *right = nullptr)
        : left(left), right(right), nodetype(nodetypes::r_value) { filling.r_value = r_value; };
    TreeNode(double *l_value, TreeNode *left = nullptr, TreeNode *right = nullptr)
        : left(left), right(right), nodetype(nodetypes::l_value) { filling.l_value = l_value; };
    // 析构函数
    ~TreeNode() { delete left, delete right; };
};

class Parser
{
    // 私有成员变量
    Scanner scanner;           // 词法分析器
    double T;                  // 当前T值
    bool looping;              // 辅助判断T是否达到边界
    double scale_x, scale_y;   // 缩放比例
    double rotate_angle;       // 旋转角度
    double origin_x, origin_y; // 原点偏移
    double from_, to_, step_;  // range and step of T
    // parsing functions
    void match_token(enum class TokenType t);
    void program();
    void statement();
    void for_statement();
    void origin_statement();
    void scale_statement();
    void rotate_statement();
    TreeNode *expression();
    TreeNode *term();
    TreeNode *factor();
    TreeNode *component();
    TreeNode *atom();
    // helper functions
    void reset_args(); // 重置origin, scale, rotate参数
    double *next_T();  // 返回下一个指向T的指针
    static void error(const Token &tk = Token(), const string &msg = "");

public:
    static std::vector<std::tuple<double, double>> point_list; // 绘图列表
    // static std::vector<std::tuple<double, double, int>> point_list; // 绘图列表
    Parser(const string &filename, int n = 4096)
        : scanner(filename, n),
          T(0.0), looping(false),
          origin_x(0), origin_y(0),
          scale_x(1), scale_y(1),
          rotate_angle(0) {};
    void run() { program(); };
};

#endif