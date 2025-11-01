#include "parser.hpp"
#include "drawer.hpp"

int main()
{
    freopen("test_output.txt", "w", stdout);
    Parser parser("test_input.txt");
    parser.run();
    return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOW);
}