#include "scanner.hpp"
#include <string>
#include <fstream>
#include <utility>

using std::ifstream;
using std::string;

char Buffer::get_data()
{
    auto data = buffer[head];
    head = (head + 1) % max_size;
    return data;
}

void Buffer::put_data(char c)
{
    buffer[tail] = c;
    tail = (tail + 1) % max_size;
}

void Buffer::back_data(char c)
{
    head = (head - 1) % max_size;
    buffer[head] = c;
}

void Buffer::clear()
{
    buffer.clear();
    head = tail = 0;
}

std::streampos Buffer::fill_buffer(ifstream &input, std::streampos &start_pos, int n = 4095)
{
    static char buf[4096] = {'\0'};         // 临时缓冲区
    static int s = 0, e = 1, remaining = 0; // 起始位置和结束位置
step:
    // 优先读取buf中剩余的字符
    if (remaining > 0)      // 有剩余字符
        if (remaining >= n) // 剩余字符足够
        {
            remaining -= n;
            for (auto i = s; i < s + n; i++)
                put_data(buf[i]);
            s += n;
            return start_pos; // 返回下次读取的位置（等于上次）
        }
        else // 剩余字符不够
        {
            for (auto i = s; i < e; i++)
                put_data(buf[i]);
            n -= remaining;
            remaining = 0;
            s = 0;
            e = 1;
        }
    else // 无剩余字符
    {
        input.seekg(start_pos);    // 定位到上次读取的位置
        input.read(buf, n);        // 读取数据到缓冲区
        auto num = input.gcount(); // 实际读取的字符数
        return input.tellg();      // 返回下次读取的位置
    }
}

bool Scanner::init(const string &input_path)
{
    if (source_path.empty() && input_path.empty())
        return false;
    if (!input_path.empty())
        source_path = input_path;
    ifstream infile(source_path);
    if (!infile.is_open()) // 检测一下是否可以正常读取
        return false;
    return ready = true;
}

Token Scanner::getToken()
{
}