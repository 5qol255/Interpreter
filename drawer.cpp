#include "drawer.hpp"
#include <Windows.h>
#include <stdexcept>

#define Error_msg(msg) MessageBox(NULL, msg, "Error", MB_ICONERROR)

// WinMain：程序入口点
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASS MyWindowClass;
    HWND hWnd;
    MSG Msg;

    // 窗口类的初始化
    MyWindowClass.style = CS_HREDRAW | CS_VREDRAW;
    MyWindowClass.lpfnWndProc = WindowProcedure;
    MyWindowClass.cbClsExtra = 0;
    MyWindowClass.cbWndExtra = 0;
    MyWindowClass.hInstance = hInstance;
    MyWindowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    MyWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    MyWindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    MyWindowClass.lpszMenuName = NULL;
    MyWindowClass.lpszClassName = "ExampleWindowClass";
    // 窗口类的注册
    if (!RegisterClass(&MyWindowClass))
    {
        MessageBox(NULL, "Failed to register window class!", "Error", MB_ICONERROR);
        throw std::runtime_error("Failed to register window class!");
        return -1; // 注册失败，返回-1
    }

    // 创建窗口
    hWnd = CreateWindow(
        "ExampleWindowClass",
        "Windows API Window - Complete Example",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        600,
        400,
        NULL,
        NULL,
        hInstance,
        NULL);
    // 检测窗口是否创建成功
    if (!hWnd)
    {
        MessageBox(NULL, "Window creation failed!", "Error", MB_ICONERROR);
        return -1;
    }

    // 显示窗口
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // 消息循环
    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    return Msg.wParam;
}

// 消息处理函数定义
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_CREATE: // 窗口创建
        break;
    case WM_PAINT:
    {
        // 绘制窗口内容
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // 绘制一些文本
        TextOut(hdc, 20, 20, "Hello, Windows API!", 19);
        TextOut(hdc, 20, 50, "This is a complete window example.", 32);
        TextOut(hdc, 20, 80, "Click the close button to exit.", 31);
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
    {
        PostQuitMessage(0);
        break;
    }
    default:
        return DefWindowProc(hWnd, msg, wp, lp);
    }
    return 0;
}
