#include "drawer.hpp"
#include <Windows.h>
#include <stdexcept>
#include <tchar.h>
#include <string>
#include <vector>
#include <tuple>

using std::wstring;

// 辅助函数，用于检查函数调用是否成功
inline void check(bool expr, const wstring &failed_msg = L"", int failed_code = 1);
// 初始化窗口类并注册过程
inline bool init_window_class(WNDCLASS *wc, HINSTANCE hInstance);
// 创建窗口过程
inline bool make_window(HWND *hwnd, HINSTANCE hInstance);
// 首次更新窗口过程
inline void first_update(HWND hWnd, int nCmdShow);
// 处理消息过程
inline void handle_messages(MSG *msg);
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

inline void draw_point(HDC hdc, int x, int y, int color)
{
    SetPixelV(hdc, x, y, color);
}

inline void draw_points(HDC hdc, const std::vector<std::tuple<int, int, int>> &points)
{
    for (auto const &point : points)
        draw_point(hdc, std::get<0>(point), std::get<1>(point), std::get<2>(point));
}

inline void check(bool expr, const wstring &failed_msg, int failed_code)
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

inline bool init_window_class(WNDCLASS *wc, HINSTANCE hInstance)
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

inline bool make_window(HWND *hwnd, HINSTANCE hInstance)
{
    *hwnd = CreateWindow(
        L"ExampleWindowClass",
        L"Windows API Window - Complete Example",
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

inline void first_update(HWND hWnd, int nCmdShow)
{
    // 显示窗口
    ShowWindow(hWnd, nCmdShow);
    // 首次绘制强制刷新
    UpdateWindow(hWnd);
}

inline void handle_messages(MSG *msg)
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
    // case WM_CREATE: // 窗口创建
    //     break;      // 无事发生
    case WM_PAINT:
    {
        // 绘制窗口内容
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // 绘制一些文本
        // TextOut(hdc, 20, 20, L"Hello, Windows API!", 19);
        // TextOut(hdc, 20, 50, L"This is a complete window example.", 32);
        // TextOut(hdc, 20, 80, L"Click the close button to exit.", 31);
        draw_point(hdc, 100, 100, RGB(255, 0, 0));
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_SIZE: // 窗口大小改变
    {
        // int width = LOWORD(lp);
        // int height = HIWORD(lp);
        // TCHAR szText[100];
        // _stprintf_s(szText, "Window resized to %d x %d", width, height);
        // 可以在这里添加窗口大小改变的处理逻辑
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
