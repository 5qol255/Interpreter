#include "drawer.hpp"
#include <stdio.h>

// 静态成员定义和初始化
Window *Window::this_ = nullptr;

// 初始化窗口
bool Window::Initialize(HINSTANCE hInstance, int nCmdShow)
{
    // 保存应用程序实例句柄到成员变量
    this->hInstance = hInstance;

    // 保存实例指针，用于在静态回调中访问
    this_ = this;

    // 设置窗口类结构体的各个字段
    windowClass = {
        CS_HREDRAW | CS_VREDRAW,         // 窗口类样式：CS_HREDRAW和CS_VREDRAW表示窗口水平或垂直改变大小时重绘整个窗口
        WindowProcedure,                 // 指定窗口过程回调函数，这是处理窗口消息的函数
        0,                               // 额外的类内存字节数，通常设置为0
        0,                               // 额外的窗口内存字节数，通常设置为0
        hInstance,                       // 应用程序实例句柄
        LoadIcon(NULL, IDI_APPLICATION), // 窗口类图标，使用系统默认应用程序图标
        LoadCursor(NULL, IDC_ARROW),     // 窗口类光标，使用系统默认箭头光标
        (HBRUSH)(COLOR_WINDOW + 1),      // 窗口背景画刷，使用系统默认窗口背景色
        NULL,                            // 菜单名称，NULL表示没有菜单
        L"ExampleWindowClass",           // 窗口类名称，用于标识这个窗口类
    };
    // 注册窗口类
    if (!RegisterClass(&windowClass))
    {
        // 如果注册失败，显示错误消息框并返回false
        MessageBox(NULL, L"Failed to register window class!", L"Error", MB_OK | MB_ICONERROR);
        return false;
    }

    // 创建实际的窗口，CreateWindow函数创建一个窗口并返回窗口句柄
    hWnd = CreateWindow(
        L"ExampleWindowClass",      // 窗口类名，必须与注册的类名相同
        L"Windows API Window demo", // 窗口标题文本
        WS_OVERLAPPEDWINDOW,        // 窗口样式，WS_OVERLAPPEDWINDOW包括标题栏、边框、系统菜单等
        CW_USEDEFAULT,              // 窗口初始位置X坐标，CW_USEDEFAULT表示由系统决定
        CW_USEDEFAULT,              // 窗口初始位置Y坐标，CW_USEDEFAULT表示由系统决定
        600,                        // 窗口宽度，600像素
        400,                        // 窗口高度，400像素
        NULL,                       // 父窗口句柄，NULL表示没有父窗口
        NULL,                       // 菜单句柄，NULL表示没有菜单
        hInstance,                  // 应用程序实例句柄
        NULL                        // 创建参数，NULL表示没有额外参数
    );
    // 检查窗口创建是否成功
    if (!hWnd)
    {
        DWORD error = GetLastError();
        printf("%lu\n", error);
        // 如果创建失败，显示错误消息框并返回false
        MessageBox(NULL, L"Window creation failed!", L"Error", MB_OK | MB_ICONERROR);
        return false;
    }

    // 显示窗口并强制重绘一次
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // 设置初始化状态标志成功并返回
    return isInitialized = true;
}

// 运行消息循环
bool Window::Run()
{
    // 检查窗口是否已成功初始化
    if (!isInitialized)
        return false;

    // Windows消息循环
    // 这是GUI应用程序的核心循环
    // GetMessage从线程消息队列获取消息
    // 当收到WM_QUIT消息时，GetMessage返回0，循环结束
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        // TranslateMessage转换某些键盘消息
        // 例如将WM_KEYDOWN和WM_KEYUP转换为WM_CHAR消息
        TranslateMessage(&msg);

        // DispatchMessage将消息分发给相应的窗口过程
        // 即调用之前注册的WindowProcedure函数
        DispatchMessage(&msg);
    }
    return true;
}

// 静态窗口过程回调函数
LRESULT CALLBACK Window::WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    // 检查静态实例指针是否有效
    if (this_)
        switch (msg)
        {
        /** 窗口创建消息,在窗口完全创建后发送，但尚未显示 */
        case WM_CREATE:
            return this_->OnCreate();
        /** 窗口重绘消息,当窗口需要重绘时发送 */
        case WM_PAINT:
            return this_->OnPaint();
        /** 窗口大小改变消息,当用户调整窗口大小或窗口状态改变时发送
            参数lp的低位和高位分别包含新的宽度和高度 */
        case WM_SIZE:
        {
            int width = LOWORD(lp);
            int height = HIWORD(lp);
            return this_->OnSize(width, height);
        }
        /** 窗口销毁消息,在窗口即将被销毁时发送
            通常在此发送WM_QUIT消息结束消息循环 */
        case WM_DESTROY:
            return this_->OnDestroy();
        /** 对于未处理的消息，调用默认处理方法
            默认处理方法会调用DefWindowProc进行系统默认处理 */
        default:
            return this_->OnDefault(msg, wp, lp);
        }
    // 如果实例不存在，使用默认处理
    else
        return DefWindowProc(hWnd, msg, wp, lp);
}

// 消息处理的默认实现 - WM_CREATE消息
LRESULT Window::OnCreate()
{
    // 窗口创建时的处理，这里没什么需要处理的，直接返回0
    return 0;
}

// 消息处理的默认实现 - WM_PAINT消息
LRESULT Window::OnPaint()
{
    // 定义PAINTSTRUCT结构体，用于存储绘制信息
    PAINTSTRUCT ps;

    // BeginPaint获取窗口的设备上下文（HDC）并准备绘制，同时使窗口的无效区域变为有效
    HDC hdc = BeginPaint(hWnd, &ps);

    // 使用TextOut函数在窗口上绘制文本
    // 参数：设备上下文、X坐标、Y坐标、文本字符串、字符串长度
    TextOut(hdc, 20, 20, L"Hello, Windows API!", 19);
    TextOut(hdc, 20, 50, L"This is a complete window example.", 32);
    TextOut(hdc, 20, 80, L"Click the close button to exit.", 31);

    // EndPaint释放设备上下文并完成绘制过程
    EndPaint(hWnd, &ps);

    return 0;
}

// 消息处理的默认实现 - WM_SIZE消息
LRESULT Window::OnSize(int width, int height)
{
    // 窗口大小改变时的处理，这里没什么需要处理的，直接返回0
    return 0;
}

// 消息处理的默认实现 - WM_DESTROY消息
LRESULT Window::OnDestroy()
{
    // 发送WM_QUIT消息，这会使GetMessage返回0，结束消息循环
    PostQuitMessage(0);
    return 0;
}

// 消息处理的默认实现 - 未处理消息
LRESULT Window::OnDefault(UINT msg, WPARAM wp, LPARAM lp)
{
    // 调用系统默认的窗口过程进行处理
    return DefWindowProc(hWnd, msg, wp, lp);
}
