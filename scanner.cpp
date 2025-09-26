#include "scanner.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <cctype>  // for isspace()
#include <cstdlib> // for stod()

using std::string;

int Buffer::get_data()
{
    // printf("%d\n", current_size);
    if (current_size < 1 && closed)
        return EOF;
    if (current_size < 1)
    {
        fill_buffer();        // 尝试填充缓冲区
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

void Buffer::fill_buffer()
{
    int n = max_size - 1;            // 本次填充缓冲区的最大字节数
    file.seekg(start_pos);           // 定位到上次读取文件结束的位置
    file.read(&buffer[0], n);        // 读取n个字节到缓冲区
    start_pos = file.tellg();        // 更新位置
    auto read_bytes = file.gcount(); // 实际读取的字节数
    current_size = read_bytes;       // 更新当前大小
    if (read_bytes < n)              // 小于n说明文件已经读到结束
    {
        closed = true; // 更改eof标记
        file.close();  // 关闭文件
    }
    // printf("Filled %d bytes into buffer\n", current_size);
}

const std::unordered_map<int, TokenType> final_states = {
    {1, TokenType::ID},
    {2, TokenType::CONST_ID},
    {3, TokenType::CONST_ID},
    {4, TokenType::MUL},
    {5, TokenType::POWER},
    {6, TokenType::DIV},
    {7, TokenType::MINUS},
    {8, TokenType::PLUS},
    {9, TokenType::COMMA},
    {10, TokenType::SEMICO},
    {11, TokenType::L_BRACKET},
    {12, TokenType::R_BRACKET},
    {13, TokenType::COMMENT},
}; // 终态表

const std::unordered_map<int, std::unordered_map<char, int>> state_transition_table = {
    {0, {
            {'0', 2},
            {'1', 2},
            {'2', 2},
            {'3', 2},
            {'4', 2},
            {'5', 2},
            {'6', 2},
            {'7', 2},
            {'8', 2},
            {'9', 2},
            {'*', 4},
            {'/', 6},
            {'-', 7},
            {'+', 8},
            {',', 9},
            {';', 10},
            {'(', 11},
            {')', 12},
            {'a', 1},
            {'b', 1},
            {'c', 1},
            {'d', 1},
            {'e', 1},
            {'f', 1},
            {'g', 1},
            {'h', 1},
            {'i', 1},
            {'j', 1},
            {'k', 1},
            {'l', 1},
            {'m', 1},
            {'n', 1},
            {'o', 1},
            {'p', 1},
            {'q', 1},
            {'r', 1},
            {'s', 1},
            {'t', 1},
            {'u', 1},
            {'v', 1},
            {'w', 1},
            {'x', 1},
            {'y', 1},
            {'z', 1},
            {'A', 1},
            {'B', 1},
            {'C', 1},
            {'D', 1},
            {'E', 1},
            {'F', 1},
            {'G', 1},
            {'H', 1},
            {'I', 1},
            {'J', 1},
            {'K', 1},
            {'L', 1},
            {'M', 1},
            {'N', 1},
            {'O', 1},
            {'P', 1},
            {'Q', 1},
            {'R', 1},
            {'S', 1},
            {'T', 1},
            {'U', 1},
            {'V', 1},
            {'W', 1},
            {'X', 1},
            {'Y', 1},
            {'Z', 1},
        }},
    {1, {
            {'a', 1},
            {'b', 1},
            {'c', 1},
            {'d', 1},
            {'e', 1},
            {'f', 1},
            {'g', 1},
            {'h', 1},
            {'i', 1},
            {'j', 1},
            {'k', 1},
            {'l', 1},
            {'m', 1},
            {'n', 1},
            {'o', 1},
            {'p', 1},
            {'q', 1},
            {'r', 1},
            {'s', 1},
            {'t', 1},
            {'u', 1},
            {'v', 1},
            {'w', 1},
            {'x', 1},
            {'y', 1},
            {'z', 1},
            {'A', 1},
            {'B', 1},
            {'C', 1},
            {'D', 1},
            {'E', 1},
            {'F', 1},
            {'G', 1},
            {'H', 1},
            {'I', 1},
            {'J', 1},
            {'K', 1},
            {'L', 1},
            {'M', 1},
            {'N', 1},
            {'O', 1},
            {'P', 1},
            {'Q', 1},
            {'R', 1},
            {'S', 1},
            {'T', 1},
            {'U', 1},
            {'V', 1},
            {'W', 1},
            {'X', 1},
            {'Y', 1},
            {'Z', 1},
            {'0', 1},
            {'1', 1},
            {'2', 1},
            {'3', 1},
            {'4', 1},
            {'5', 1},
            {'6', 1},
            {'7', 1},
            {'8', 1},
            {'9', 1},
        }},
    {2, {
            {'.', 3},
            {'0', 2},
            {'1', 2},
            {'2', 2},
            {'3', 2},
            {'4', 2},
            {'5', 2},
            {'6', 2},
            {'7', 2},
            {'8', 2},
            {'9', 2},
        }},
    {3, {
            {'0', 3},
            {'1', 3},
            {'2', 3},
            {'3', 3},
            {'4', 3},
            {'5', 3},
            {'6', 3},
            {'7', 3},
            {'8', 3},
            {'9', 3},
        }},
    {4, {
            {'*', 5},
        }},
    {6, {
            {'/', 13},
        }},
    {7, {
            {'-', 13},
        }},
}; // 状态转移表

int smove(int current_state, char c)
{
    try // 尝试状态转移
    {
        return state_transition_table.at(current_state).at(c);
    }
    catch (std::out_of_range &e) // 表中没有说明是无效状态
    {
        return -1;
    }
}

TokenType get_final_state_type(int state)
{
    try
    {
        return final_states.at(state);
    }
    catch (std::out_of_range &e)
    {
        return TokenType::ERROR;
    }
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
        token.name += char(std::toupper(ch)); // 追加字符
        state = next_state;                   // 更新状态
        ch = buffer.get_data();               // 获取下一个字符
        if (ch == EOF)                        // 文件结束
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