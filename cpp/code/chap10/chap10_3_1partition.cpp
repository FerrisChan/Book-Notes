//
// Created by ferris on 17-6-1.
//

// 使用谓词，并结合partiton： 输入一个string，答应长度大于5的string
// 输入的文件为 text.txt

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

inline void showVector ( vector<string> &vect) {
    for (auto iter = vect.begin();iter != vect.end();++iter) {
        cout << *iter << " ";
    }
    cout<<endl;
}

bool isFive(string &string1){
    // 返回size是否比5大的bool值

    return string1.size() >= 5;
}


int main(int argc, char *argv[]) {
    ifstream in(argv[1]);
    if(!in) {
        cout<<"打开文件失败"<<endl;
        exit(-1);
    }

    vector<string> vect;  // 输入的容器
    string str;           // vect的元素
    while (in>> str){
        vect.push_back(str);
    }
    cout<< "源文件为:" << endl;
    showVector(vect);
    auto iterBack = partition(vect.begin(),vect.end(),isFive);
    cout<< "设置后为:"<<endl;
    for (auto iter = vect.begin();iter != iterBack;++iter) {
        cout << *iter << " ";
    }
    cout<<endl;
    return 0;


}