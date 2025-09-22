#include "scanner.hpp"
#include "dfa.hpp"
#include <string>
#include <fstream>
#include <cctype>  // for isspace()
#include <cstdlib> // for stod()

using std::ifstream;
using std::string;

int Buffer::get_data()
{
    // printf("%d\n", current_size);
    if (current_size < 1 && eof)
        return EOF;
    if (current_size < 1)
    {
        fill_buffer(file, start_pos, max_size);
        head = 0;             // 重置head指针
        if (current_size < 1) // 仍然没有数据，说明文件结束
            return EOF;
    }
    // printf("Buffer: %d\n", int(buffer[head++]));
    --current_size;
    return buffer[head++];
}

void Buffer::back_data()
{
    --head;
    ++current_size;
}

void Buffer::clear()
{
    buffer.clear();
    head = current_size = 0;
}

void Buffer::fill_buffer(ifstream &input, std::streampos &pos, int n)
{
    input.seekg(pos);                 // 定位到上次读取文件结束的位置
    input.read(&buffer[0], n);        // 读取n个字节到缓冲区
    pos = input.tellg();              // 更新位置
    auto read_bytes = input.gcount(); // 实际读取的字节数
    current_size = read_bytes;        // 更新当前大小
    if (read_bytes < n)               // 小于n说明文件已经读到结束
        eof = true;                   // 更改eof标记
    // printf("Filled %d bytes into buffer\n", current_size);
}

Token Scanner::getToken()
{
    int state = 0, next_state;
    int ch = buffer.get_data();
    // printf("state: %d, ch: %c\n", state, ch);
    while (std::isspace(ch)) // 跳过空白字符
        ch = buffer.get_data();
    if (ch == EOF) // 文件结束
        return Token(TokenType::END, "END");
    // printf("ch: %c==%d\n", ch, ch);
    Token token;
    for (;;)
    {
        next_state = smove(state, char(ch));
        if (next_state == -1) // 没有下一个有效转移了
        {
            buffer.back_data(); // 回退字符
            break;              // 退出循环
        }
        token.name += char(ch); // 追加字符
        state = next_state;     // 更新状态
        ch = buffer.get_data(); // 获取下一个字符
        if (ch == EOF)          // 文件结束
            break;
    }
    token.type = get_final_state_type(state);
    // printf("Final state: %d, Token type: %d\n", state, int(token.type));
    switch (token.type)
    {
    case TokenType::ID:
        break;
    case TokenType::CONST_ID:
        token.value.v = std::stod(token.name);
        break;
    case TokenType::COMMENT:
        int c;
        while ((c = buffer.get_data()) != EOF && c != '\n')
            ; // 跳过注释
        break;
    default:
        break;
    }
    return token;
}