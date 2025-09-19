// #include "scanner.hpp"
#include <vector>
#include <stdio.h>
using namespace std;
int main()
{
    vector<char> v(1024);
    printf("%d %d", v.size(), v.capacity());
    return 0;
}