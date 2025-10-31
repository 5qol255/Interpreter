// test_procedural.cpp
#define UNICODE  // 定义UNICODE宏以支持Unicode字符集
#define _UNICODE // 同上
#include <Windows.h>
#include <tchar.h>
#include <stdio.h>

// 全局变量
HINSTANCE g_hInstance = NULL;
HWND g_hWnd = NULL;
BOOL g_isInitialized = FALSE;

// 函数声明
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
BOOL Initialize(HINSTANCE hInstance, int nCmdShow);
BOOL Run();
LRESULT OnCreate();
LRESULT OnPaint();
LRESULT OnSize(int width, int height);
LRESULT OnDestroy();
LRESULT OnDefault(UINT msg, WPARAM wp, LPARAM lp);

// 主函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    g_hInstance = hInstance;

    if (!Initialize(hInstance, nCmdShow))
    {
        MessageBox(NULL, L"Initialization failed!", L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    Run();

    return 0;
}

// 初始化窗口
BOOL Initialize(HINSTANCE hInstance, int nCmdShow)
{
    WNDCLASS windowClass = {0};

    // 设置窗口类结构体的各个字段
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowProcedure;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = hInstance;
    windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = L"ProceduralWindowClass";

    // 注册窗口类
    if (!RegisterClass(&windowClass))
    {
        DWORD error = GetLastError();
        printf("RegisterClass failed with error: %lu\n", error);
        MessageBox(NULL, L"Failed to register window class!", L"Error", MB_OK | MB_ICONERROR);
        return FALSE;
    }

    // 创建实际的窗口
    g_hWnd = CreateWindow(
        L"ProceduralWindowClass",         // 窗口类名
        L"Procedural Windows API Window", // 窗口标题
        WS_OVERLAPPEDWINDOW,              // 窗口样式
        CW_USEDEFAULT,                    // 窗口初始位置X坐标
        CW_USEDEFAULT,                    // 窗口初始位置Y坐标
        600,                              // 窗口宽度
        400,                              // 窗口高度
        NULL,                             // 父窗口句柄
        NULL,                             // 菜单句柄
        hInstance,                        // 应用程序实例句柄
        NULL                              // 创建参数
    );

    // 检查窗口创建是否成功
    if (!g_hWnd)
    {
        DWORD error = GetLastError();
        printf("CreateWindow failed with error: %lu\n", error);
        MessageBox(NULL, L"Window creation failed!", L"Error", MB_OK | MB_ICONERROR);
        return FALSE;
    }

    // 显示窗口并强制重绘一次
    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);

    g_isInitialized = TRUE;
    return TRUE;
}

// 运行消息循环
BOOL Run()
{
    if (!g_isInitialized)
        return FALSE;

    MSG msg;

    // Windows消息循环
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return TRUE;
}

// 窗口过程回调函数
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_CREATE:
        return OnCreate();
    case WM_PAINT:
        return OnPaint();
    case WM_SIZE:
    {
        int width = LOWORD(lp);
        int height = HIWORD(lp);
        return OnSize(width, height);
    }
    case WM_DESTROY:
        return OnDestroy();
    default:
        return OnDefault(msg, wp, lp);
    }
}

// 处理WM_CREATE消息
LRESULT OnCreate()
{
    return 0;
}

// 处理WM_PAINT消息
LRESULT OnPaint()
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(g_hWnd, &ps);

    // 绘制文本
    TextOut(hdc, 20, 20, L"Hello, Procedural Windows API!", 27);
    TextOut(hdc, 20, 50, L"This is a procedural window example.", 36);
    TextOut(hdc, 20, 80, L"Click the close button to exit.", 31);

    EndPaint(g_hWnd, &ps);
    return 0;
}

// 处理WM_SIZE消息
LRESULT OnSize(int width, int height)
{
    return 0;
}

// 处理WM_DESTROY消息
LRESULT OnDestroy()
{
    PostQuitMessage(0);
    return 0;
}

// 处理默认消息
LRESULT OnDefault(UINT msg, WPARAM wp, LPARAM lp)
{
    return DefWindowProc(g_hWnd, msg, wp, lp);
}
