
//
// Created by ferris on 17-5-21.
//  从标准输入中读取若干个string对象并查找重复出现的单词
//
#include <iostream>
#include <typeinfo>
#include <vector>
#include <string>

using namespace std;
int main() {
    string currString, preString = "", maxString;
    int currCnt = 1, maxCnt = 0;

    cout << "输入y or Y 结束判断"<< endl;
    while (cin >> currString){
        // 当前string等于上一个string
        if (currString == preString){
            ++currCnt;
            if (currCnt > maxCnt){
                maxCnt = currCnt;
                maxString = currString;
            }

            else{
                currCnt= 1;
            }
        }

        // 更新preString,下一次使用
        preString = currString;

        if (currString == "y" ||currString == "Y")  break;

    }
    if (maxCnt > 1){
        cout << "出现最多的字符串为"<< maxString;
        cout << "次数为"<<maxCnt<<endl;
    }

    else
        cout <<"每个字符串只出现一次";
    return 0;
}
