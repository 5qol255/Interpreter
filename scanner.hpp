#ifndef SCANNER_HPP
#define SCANNER_HPP
#pragma once

#include <string>
#include <list>
#include <fstream>
#include <vector>
#include <math.h>

using std::ifstream;
using std::list;
using std::string;
using std::vector;

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
    ERROR, // 非法记号
};

class Token // 记号类
{
public:
    /* 成员变量 */
    TokenType type;
    string name;
    union
    {
        double v;
        double (*func_ptr)(double);
    } value;
    /* 构造函数 */
    Token(TokenType t, const char n[]) : type(t), name(n) {};
    Token(TokenType t, const char n[], double v) : Token(t, n)
    {
        value.v = v;
    };
    Token(TokenType t, const char n[], double (*fp)(double)) : Token(t, n)
    {
        value.func_ptr = fp;
    };
};

static const Token TOKENTABLE[] = {
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
    /* 以下均在math.h中 */
    {TokenType::CONST_ID, "PI", M_PI},
    {TokenType::CONST_ID, "E", M_E},
    {TokenType::FUNC, "SIN", sin},
    {TokenType::FUNC, "COS", cos},
    {TokenType::FUNC, "TAN", tan},
    {TokenType::FUNC, "LN", log},
    {TokenType::FUNC, "EXP", exp},
    {TokenType::FUNC, "SQRT", sqrt},
};

class Buffer
{
    vector<char> buffer;
    int head, tail;
    int max_size;

public:
    Buffer(int size = 4096) : head(0), tail(0), max_size(size), buffer(size) {};
    bool is_empty() { return head == tail; };
    bool is_full() { return (tail + 1) % max_size == head; };
    char get_data();
    void put_data(char c);
    void back_data(char c);
    void clear();
    std::streampos fill_buffer(ifstream &input, std::streampos &start_pos, int n = 2048);
};

class Scanner
{
    list<string> lines;
    string source_path;
    bool ready;
    list<string>::iterator current_char;
    int current_line;

public:
    Scanner(const string &source_path = "") : source_path(source_path), ready(false) {};
    bool init(const string &input_path = ""); // 初始化和预处理
    bool shutdown();
    Token getToken();
};

#endif // SCANNER_HPP