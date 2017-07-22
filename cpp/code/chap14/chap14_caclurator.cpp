//
// Created by ferris on 17-7-22.
//  简单的桌面计算器


#include <iostream>
#include <string>
#include <map>
#include <functional>

int add(int i, int j) { return i + j; }
auto mod = [](int i, int j) { return i % j; };
struct Div { int operator ()(int i, int j) const { return i / j; } };

// 函数表
auto binops = std::map<std::string, std::function<int(int, int)>>
        {
                { "+", add },                               // 函数指针
                { "-", std::minus<int>() },                 // 调用标准库function
                { "/", Div() },                             // 函数对象
                { "*", [](int i, int j) { return i*j; } },  // lambda
                { "%", mod }                                //  lambda
        };


int main()
{
    while (std::cout << "Pls enter as: num operator num :\n", true)
    {
        int lhs, rhs; std::string op;
        std::cin >> lhs >> op >> rhs;
        std::cout << binops[op](lhs, rhs) << std::endl;
    }

    return 0;
}

