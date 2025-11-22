#ifndef DRAWER_HPP
#define DRAWER_HPP

#pragma once

#define UNICODE
#define _UNICODE

#include "parser.hpp"
#include <Windows.h>
#include <vector>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
void draw_point(HDC hdc, int x, int y, int color = 0x000000ff);
// void draw_points(HDC hdc, std::vector<Point> &points);
void draw_points(HDC hdc, const std::vector<Point> &points);

#endif // DRAWER_HPP