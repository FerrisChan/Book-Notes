//
// Created by ferris on 17-7-23.
// 定义自己的函数模板 compare

#include <iostream>
using namespace std;

template <typename t>      // 注意分号

int compare(const t &v1, const t &v2)
{
    if (v1 > v2) return 1;
    if (v1 < v2) return -1;
    return 0;
}

int main()
{
    cout << compare(1,0) <<endl;
    cout << compare('a','b') << endl;



}



