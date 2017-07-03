//
// Created by ferris on 17-7-1.
//


//熟悉动态内存的new 和 delete的shiyong
//定义一个动态分配的int的vector，然后读取输入和打印，最后使用delete删除

#include <iostream>
#include <vector>


using namespace std;

vector<int> *foo(){
    auto *pv = new (nothrow) vector<int>;
    return pv;

}

vector<int> read(vector<int> *pv){
    int temp ;
    while(cin>> temp){
        pv->push_back(temp);
    }
}

void print(vector<int> *pv){
    for (auto &v : *pv )
        cout<< v << " ";
    cout<<endl;
}

int main(int argc, char *argv[]) {
    vector<int> *pv = foo();
    read(pv);
    print(pv);
    delete pv;
    pv = nullptr;
    return  0;
}
