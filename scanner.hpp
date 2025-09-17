#include <string>
using std::string;

enum class TokenType // 枚举记号类型
{
    ID,       // 标识符
    COMMENT,  // 注释
    CONST_ID, // 数值常量
    FUNC,     // 函数
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
    END,      // 结束符
    NONTOKEN, // 非记号
};

class Token // 记号类
{
public:
    // 成员变量
    TokenType type;
    union attribute
    {
        char name[8];
        double value;
        double (*func_ptr)(double);
    } attr;
    // 构造函数
    Token(TokenType t, const char *n) : type(t)
    {
        memcpy(attr.name, n, sizeof(n) - 1); // 拷贝字符串
        attr.name[sizeof(n) - 1] = '\0';     // 手动加上结束符
    };
    Token(TokenType t, double v) : type(t) { attr.value = v; };
    Token(TokenType t, double (*fp)(double)) : type(t) { attr.func_ptr = fp; };
};