#include "dfa.hpp"
#include "scanner.hpp"
#include <string>

using std::string;

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