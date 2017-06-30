//
// Created by ferris on 17-6-14.
//

// 读入string 和int序列，将每个sring和int存入一个pair中，pair保存在一个vector中

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]) {
    vector<pair<string,int>>   data;  //pair保存在一个vector中
    string s;
    int v;
    ifstream in(argv[1]);
    while (in >> s and in >>v){      //读取string和int
        data.push_back(pair<string,int>(s,v));
    }
    for(auto &d:data ){
        cout<< d.first<<" "<<d.second<<endl;
    }
    return 0;
}