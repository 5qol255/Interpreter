#ifndef DFA_HPP
#define DFA_HPP

#pragma once

#include "scanner.hpp"
#include <unordered_map>

using std::unordered_map;

static const unordered_map<int, TokenType> final_states = {
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

static const unordered_map<int, unordered_map<char, int>> state_transition_table = {
    {0, {
            {'*', 4},
            {'/', 6},
            {'-', 7},
            {'+', 8},
            {',', 9},
            {';', 10},
            {'(', 11},
            {')', 12},
            {'a', 3},
            {'b', 3},
            {'c', 3},
            {'d', 3},
            {'e', 3},
            {'f', 3},
            {'g', 3},
            {'h', 3},
            {'i', 3},
            {'j', 3},
            {'k', 3},
            {'l', 3},
            {'m', 3},
            {'n', 3},
            {'o', 3},
            {'p', 3},
            {'q', 3},
            {'r', 3},
            {'s', 3},
            {'t', 3},
            {'u', 3},
            {'v', 3},
            {'w', 3},
            {'x', 3},
            {'y', 3},
            {'z', 3},
            {'A', 3},
            {'B', 3},
            {'C', 3},
            {'D', 3},
            {'E', 3},
            {'F', 3},
            {'G', 3},
            {'H', 3},
            {'I', 3},
            {'J', 3},
            {'K', 3},
            {'L', 3},
            {'M', 3},
            {'N', 3},
            {'O', 3},
            {'P', 3},
            {'Q', 3},
            {'R', 3},
            {'S', 3},
            {'T', 3},
            {'U', 3},
            {'V', 3},
            {'W', 3},
            {'X', 3},
            {'Y', 3},
            {'Z', 3},
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

int smove(int current_state, char c);      // 状态转移函数
TokenType get_final_state_type(int state); // 返回终态种类

#endif // DFA_HPP