#ifndef SCANNER_HPP
#define SCANNER_HPP
#pragma once

#include <string>
#include <fstream>
#include <vector>

using std::string;

enum class TokenType // 枚举记号类型
{
    ID,       // 标识符
    COMMENT,  // 注释
    CONST_ID, // 数值常量
    FUNC,     // 函数
    T,        // 参数
    /* 分隔符 */
    SEMICO,    // ;
    L_BRACKET, // (
    R_BRACKET, // )
    COMMA,     // ,
    /* 运算符 */
    PLUS,  // +
    MINUS, // -
    MUL,   // *
    DIV,   // /
    POWER, // **
    /* 关键字 */
    ORIGIN, // 原点
    SCALE,  // 缩放
    ROTATE, // 旋转
    IS,     // 是
    FROM,   // 从
    TO,     // 到
    STEP,   // 步长
    FOR,    // 循环
    DRAW,   // 绘制
    /* 其他 */
    END,   // 结束符
    Error, // 非法记号
};

class Token // 记号类
{
public:
    /* 成员变量 */
    enum class TokenType type;
    string name;
    union
    {
        double v;
        double (*func_ptr)(double);
    } value;
    /* 构造函数 */
    Token(enum class TokenType t = TokenType::Error, const char n[] = "") : type(t), name(n) {};
    const string &to_str() const { return name; };
    // void print() { printf("%s %s\n", type, name.c_str()); };
};

class Buffer
{
    std::vector<char> buffer;
    int head;
    int max_size;
    int current_size;
    std::ifstream file;
    std::streampos start_pos;
    bool closed;
    void fill_buffer();

public:
    Buffer(const string &path, int size = 4096)
        : head(0), max_size(size), current_size(0), closed(false),
          buffer(size), start_pos(std::streampos(0))
    {
        file.open(path, std::ios::in);
        if (!file.is_open())
            throw std::runtime_error("Failed to open file: " + path);
        fill_buffer(); // 填充缓冲区
    };
    int get_data(); // 返回int兼容EOF
    void back_data();
    void clear();
};

class Scanner
{
    Buffer buffer;

public:
    Scanner(string filepath, int size = 4096) : buffer(filepath, size) {};
    Token getToken(); // 获取下一个记号
};

#endif // SCANNER_HPP