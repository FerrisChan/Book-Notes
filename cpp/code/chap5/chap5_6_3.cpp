//
// 程序作用： 输出两个数相除结果，并使用try去捕捉异常
// Created by ferris on 17-5-21.
//

#include <iostream>

using namespace std;
int main() {
    float num1 , num2;
    while ( cin >> num1 >> num2){
        try {
            // 捕捉到异常
            if (num2 == 0)
                throw runtime_error("num2 cannot be zero!");
            cout << num1/num2 <<endl ;
        }catch (runtime_error err /* 异常声明 */){

            // 异常发生后需要执行的语句
            cout << err.what()<<endl;
            cout << "需要继续吗"<<endl;
            char ch;
            cin >> ch;
            if (ch !='y' && ch != 'Y')
                break;
        }

    }
    return 0;

}