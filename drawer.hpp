#ifndef WINDOW_H
#define WINDOW_H

#pragma once

#define UNICODE      // 定义UNICODE宏以支持Unicode字符集
#define _UNICODE     // 同上
#include <Windows.h> // Windows的api
#include <tchar.h>   // TCHAR类型定义，支持Unicode和ANSI编译

class Window
{
private:
    /** 静态窗口过程回调函数
        Windows系统通过此函数向窗口发送消息
        由于Windows API要求回调函数必须是静态函数或全局函数，
        因此使用此静态函数接收消息，然后转发给实例的成员方法
        hWnd 窗口句柄，标识接收消息的窗口
        msg 消息标识符，如WM_PAINT、WM_DESTROY等
        wp 消息的附加参数（高位），通常为消息相关数据
        lp 消息的附加参数（低位），通常为消息相关数据
        返回值根据消息类型而定，某些消息需要特定返回值 */
    static LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
    /** 指向当前窗口实例的静态指针
        由于WindowProcedure是静态函数，无法直接访问类的非静态成员
        通过此静态指针，静态回调函数可以访问具体的窗口实例
        在Initialize方法中设置为当前对象指针，在Cleanup中重置为nullptr */
    static Window *this_;

    // 窗口相关成员变量
    HINSTANCE hInstance;  // 应用程序实例句柄，用于标识当前进程
    HWND hWnd;            // 窗口句柄，唯一标识创建的窗口
    WNDCLASS windowClass; // 窗口类结构，定义窗口的基本属性
    MSG msg;              // 消息结构，用于存储从消息队列获取的消息
    bool isInitialized;   // 初始化状态标志，指示窗口是否已成功初始化

public:
    /* 构造函数初始化窗口对象的成员变量，但不创建实际窗口 */
    Window() : hInstance(nullptr), hWnd(nullptr), isInitialized(false)
    {
        memset(&windowClass, 0, sizeof(WNDCLASS));
        memset(&msg, 0, sizeof(MSG)); // 对这两个成员变量进行清零
    };
    /* 析构函数，释放窗口资源 */
    ~Window()
    {
        if (isInitialized)
        {
            isInitialized = false;
            this_ = nullptr;
        }
    };

    /** 初始化窗口
        hInstance 应用程序实例句柄，通常从WinMain传递
        nCmdShow 窗口显示方式，如SW_SHOW、SW_HIDE等
        成功返回true，失败返回false
        失败原因可能包括：窗口类注册失败、窗口创建失败等 */
    bool Initialize(HINSTANCE hInstance, int nCmdShow);

    /** 运行消息循环
        启动Windows消息循环，这是GUI应用程序的核心
        消息循环持续运行直到收到WM_QUIT消息
        在循环中获取、翻译、分发消息给窗口过程
        成功返回true，如果窗口未初始化则返回false */
    bool Run();

    // 消息处理方法
    /** 处理WM_CREATE消息
        窗口创建时的消息处理
        可以在此方法中进行窗口初始化操作
        如创建子窗口、分配资源等
        return消息处理结果，通常返回0 */
    virtual LRESULT OnCreate();

    /** 处理WM_DESTROY消息
        窗口重绘时的消息处理
        实现窗口内容的绘制逻辑
        包括文本输出、图形绘制等
        return消息处理结果，通常返回0 */
    virtual LRESULT OnPaint();

    /** 处理WM_SIZE消息
        窗口大小改变时的消息处理
        接收新的窗口尺寸，可以进行布局调整
        width 窗口的新宽度
        height 窗口的新高度
        return 消息处理结果，通常返回0 */
    virtual LRESULT OnSize(int width, int height);

    /** 处理WM_DESTROY消息
        窗口销毁时的消息处理
        通常在此发送WM_QUIT消息结束消息循环
        可以在此进行资源清理操作
        return 消息处理结果 */
    virtual LRESULT OnDestroy();

    /** 处理默认消息
        处理未被其他方法处理的消息
        调用DefWindowProc进行默认处理
        这是Windows系统对未处理消息的默认响应
        msg 消息标识符
        wp 消息参数高位
        lp 消息参数低位
        return 消息处理结果 */
    virtual LRESULT OnDefault(UINT msg, WPARAM wp, LPARAM lp);

    // 公共访问方法
    /** 获取窗口句柄
        返回当前窗口的句柄 */
    HWND GetHWND() const { return hWnd; }

    /** 检查窗口初始化状态
        如果窗口已初始化返回true，否则返回false */
    bool IsInitialized() const { return isInitialized; }
};

#endif // WINDOW_H
