#include "scanner.hpp"
#include <iostream>
using namespace std;

int main()
{
    Buffer buffer("test.cpp");
    Scanner scanner(buffer);
    auto token = scanner.getToken();
    return 0;
}