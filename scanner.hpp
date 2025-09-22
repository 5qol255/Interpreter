#ifndef SCANNER_HPP
#define SCANNER_HPP
#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <math.h>
// #include <unordered_map>

using std::ifstream;
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
    Token() {};
    Token(const Token &token) : type(token.type), name(token.name), value(token.value) {};
    Token(TokenType t, const char n[]) : type(t), name(n) {};
    Token(TokenType t, const char n[], double v) : Token(t, n)
    {
        value.v = v;
    };
    Token(TokenType t, const char n[], double (*fp)(double)) : Token(t, n)
    {
        value.func_ptr = fp;
    };
    // void print() { printf("%s %s\n", type, name.c_str()); };
};

// static const std::unordered_map<TokenType, Token> TOKENTABLE = {
//     {TokenType::END, {TokenType::END, "END"}},
//     {TokenType::ERROR, {TokenType::ERROR, "ERROR"}},
//     {TokenType::T, {TokenType::T, "T"}},
//     {TokenType::ORIGIN, {TokenType::ORIGIN, "ORIGIN"}},
//     {TokenType::SCALE, {TokenType::SCALE, "SCALE"}},
//     {TokenType::ROTATE, {TokenType::ROTATE, "ROTATE"}},
//     {TokenType::IS, {TokenType::IS, "IS"}},
//     {TokenType::FROM, {TokenType::FROM, "FROM"}},
//     {TokenType::TO, {TokenType::TO, "TO"}},
//     {TokenType::STEP, "STEP"},
//     {TokenType::FOR, "FOR"},
//     {TokenType::DRAW, "DRAW"},
//     /* 以下均在math.h中 */
//     {TokenType::CONST_ID, "PI", M_PI},
//     {TokenType::CONST_ID, "E", M_E},
//     {TokenType::FUNC, "SIN", sin},
//     {TokenType::FUNC, "COS", cos},
//     {TokenType::FUNC, "TAN", tan},
//     {TokenType::FUNC, "LN", log},
//     {TokenType::FUNC, "EXP", exp},
//     {TokenType::FUNC, "SQRT", sqrt},
// };

class Buffer
{
    std::vector<char> buffer;
    int head;
    int max_size;
    int current_size;
    string filepath;
    ifstream file;
    std::streampos start_pos;
    bool eof;
    void fill_buffer(ifstream &input, std::streampos &pos, int n = 4095);

public:
    Buffer(const string &path, int size = 4096)
        : head(0), max_size(size), current_size(0), eof(false),
          buffer(size), filepath(path), start_pos(std::streampos(0))
    {
        file.open(filepath, std::ios::in);
        if (!file.is_open())
            throw std::runtime_error("Failed to open file: " + filepath);
        fill_buffer(file, start_pos, 4096);
    };
    int get_data(); // 返回int兼容EOF
    void back_data();
    void clear();
};

class Scanner
{
    std::vector<Token> tokens;
    Buffer &buffer;

public:
    Scanner(Buffer &buf) : tokens(), buffer(buf) {};
    Token getToken(); // 获取下一个记号
};

#endif // SCANNER_HPP