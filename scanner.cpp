#include "scanner.hpp"
#include "dfa.hpp"
#include <string>
#include <fstream>
#include <utility>

using std::ifstream;
using std::string;

int Buffer::get_data()
{
    if (eof)
        return EOF;
    if (current_size < 1)
    {
        auto input = ifstream(filepath);
        fill_buffer(input, start_pos, max_size);
        head = 0; // 重置head指针
    }
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
}

Token Scanner::getToken()
{
    int state = 0, next_state;
    int ch = buffer.get_data();
    bool continuing = true;
    Token token;
    while (continuing)
    {
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
        continuing = false;
        switch (token.type)
        {
        case TokenType::ID:
            break;
        case TokenType::CONST_ID:
            token.value.v = stod(token.name);
            break;
        case TokenType::COMMENT:
            int c;
            while ((c = buffer.get_data()) != EOF && c != '\n')
                ; // 跳过注释
            continuing = true;
            break;
        }
    }
    return token;
}