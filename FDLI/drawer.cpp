#include "drawer.hpp"
#include "parser.hpp"
#include <Windows.h>
#include <stdexcept>
#include <tchar.h>
#include <string>
#include <vector>
#if DEBUG >= 1
#include <iostream>
#endif

using std::wstring;

// 辅助函数，用于检查函数调用是否成功
void check(bool expr, const wstring &failed_msg = L"", int failed_code = 1);
// 初始化窗口类并注册过程
bool init_window_class(WNDCLASS *wc, HINSTANCE hInstance);
// 创建窗口过程
bool make_window(HWND *hwnd, HINSTANCE hInstance);
// 首次更新窗口过程
void first_update(HWND hWnd, int nCmdShow);
// 处理消息过程
void handle_messages(MSG *msg);
// 消息处理函数
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

// WinMain：程序入口点
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASS MyWindowClass;
    HWND hWnd;
    MSG Msg;
    // 调用函数初始化窗口类并检查
    check(init_window_class(&MyWindowClass, hInstance), L"Failed to register window class!");
    // 调用函数创建窗口并检查
    check(make_window(&hWnd, hInstance), L"Failed to create window!", 2);
    // 调用函数首次更新窗口
    first_update(hWnd, nCmdShow);
    // 消息循环
    handle_messages(&Msg);
    return Msg.wParam;
}

void draw_point(HDC hdc, int x, int y, int color)
{
    // 绘制一个点（颜色为红色）
    SetPixelV(hdc, x, y, color);
}

void draw_points(HDC hdc, const std::vector<Point> &points)
{
    // 绘制一系列点（颜色为红色）
    for (auto const &point : points)
    {
#if DEBUG >= 1
        std::cout << point.x << ' ' << point.y << '\n';
#endif
        draw_point(hdc, point.x, point.y);
    }
    // 画完清空点集，清空会导致重绘（改变窗口大小）时没点可画变成空白
    // 除非窗口大小不改变，否则不要清空
    // 且应在非const函数中修改point_list
    // points.clear();
}

void check(bool expr, const wstring &failed_msg, int failed_code)
{
    if (!expr)
    {
        MessageBox(
            NULL,
            (failed_msg + L"\nError code: " + std::to_wstring(failed_code)).c_str(),
            L"Error",
            MB_ICONERROR);
        exit(failed_code);
    }
}

bool init_window_class(WNDCLASS *wc, HINSTANCE hInstance)
{
    wc->style = CS_HREDRAW | CS_VREDRAW;
    wc->lpfnWndProc = WindowProcedure;
    wc->cbClsExtra = 0;
    wc->cbWndExtra = 0;
    wc->hInstance = hInstance;
    wc->hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc->hCursor = LoadCursor(NULL, IDC_ARROW);
    wc->hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc->lpszMenuName = NULL;
    wc->lpszClassName = L"ExampleWindowClass";
    if (!RegisterClass(wc))
        return false;
    else
        return true;
}

bool make_window(HWND *hwnd, HINSTANCE hInstance)
{
    *hwnd = CreateWindow(
        L"ExampleWindowClass",
        L"FDLI",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        600,
        400,
        NULL,
        NULL,
        hInstance,
        NULL);
    if (!*hwnd)
        return false;
    else
        return true;
}

void first_update(HWND hWnd, int nCmdShow)
{
    // 显示窗口
    ShowWindow(hWnd, nCmdShow);
    // 首次绘制强制刷新
    UpdateWindow(hWnd);
}

void handle_messages(MSG *msg)
{
    while (GetMessage(msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(msg);
        DispatchMessage(msg);
    }
}

// 消息处理函数定义
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_PAINT:
    {
        // 绘制窗口内容
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // 绘制点
        draw_points(hdc, Parser::point_list);
        // 结束绘制
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY: // 窗口销毁时的处理
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, msg, wp, lp);
    }
    return 0;
}
