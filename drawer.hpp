#ifndef DRAWER_HPP
#define DRAWER_HPP

#pragma once

#define UNICODE
#define _UNICODE

#include <Windows.h>
#include <vector>
#include <tuple>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
inline void draw_point(HDC hdc, int x, int y, int color);
inline void draw_points(HDC hdc, const std::vector<std::tuple<int, int, int>> &points);

#endif // DRAWER_HPP