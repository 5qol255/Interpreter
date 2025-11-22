#include "FDLI/scanner.hpp"
#include "FDLI/parser.hpp"
#include "FDLI/drawer.hpp"

int main()
{
#if DEBUG >= 1
    freopen("test_output.txt", "w", stdout);
#endif
    // 创建分析器
    Parser parser("test_input.txt");
    // 运行分析器
    parser.run();
    // 绘制图像
    return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOW);
}