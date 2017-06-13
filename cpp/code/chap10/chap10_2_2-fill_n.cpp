//
// Created by ferris on 17-5-31.
//
// 使用fill_n将一个序列中的int值都设置为0
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;


int main(int argc, char *argv[]) {
    ifstream in(argv[1]);
    if(!in) {
        cout<<"打开文件失败"<<endl;
        exit(-1);
    }

    vector<int> vect;  // 输入的容器，int型
    int num;           // vect的元素
    while (in>> num){
        // 输入数字继续，输入字母就将缓冲区的列表push进vector
        vect.push_back(num);
    }
    cout<< "源文件为:" << endl;
    for (auto iter = vect.begin();iter != vect.end();++iter)
    {
        cout << *iter << " ";
    }
    cout<<endl;
    fill_n(vect.begin(),vect.size(),0);
    cout<< "设置为0后:"<<endl;
    for (auto iter = vect.begin();iter != vect.end();++iter)
    {
        cout << *iter << " ";
    }

    return 0;


}