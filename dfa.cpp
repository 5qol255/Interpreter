#include "dfa.hpp"
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