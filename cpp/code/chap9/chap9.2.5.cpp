//
// Created by ferris on 17-5-27.
//
// 将list容器中的char * 指针赋值为vector中的string

#include <iostream>
#include <list>
#include <vector>

using namespace std;



int main() {

    list< char *> a={"hello","world"};
    vector<string> b;
    auto beg = a.begin();
    auto ending = a.end();
    b.assign(beg,ending);
    for(auto beg = b.begin();beg!= b.end();++beg){
        cout<< *beg<<" ";
    }
    cout<<endl;
    return 0;

}