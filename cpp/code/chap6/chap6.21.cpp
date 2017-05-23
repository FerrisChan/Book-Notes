//
// Created by ferris on 17-5-22.
//编写自己的比较函数，其中输入一个为int，另一个为指针
#include <iostream>

using namespace std;

int myCompare (int num1, int *num2)
{
    if (num1 > *num2)
        return num1;
    return *num2;
}
int main() {
    int number1 = 5;
    int number2 = 10
    cout<< compare(number1,&number2)<<endl;

    return 0;

}
