#include "FDLI/parser.hpp"
#include "FDLI/drawer.hpp"

int main(int argc, char *argv[])
{
#if DEBUG >= 1
    if (argc > 2)
        freopen(argv[2], "w", stdout);
    else
        freopen("FDLI_log.txt", "w", stdout);
#endif
    char *filename;
    if (argc > 1)
        filename = argv[1];
    else
        filename = "FDLI.txt";
    // 创建分析器
    Parser parser(filename);
    // 运行分析器
    parser.run();
    // 绘制图像
    return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOW);
}