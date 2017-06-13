//
// Created by ferris on 17-6-2.
//

// 使用匿名函数lambda，并结合可变函数mutable
// 捕获变量并递减,直到变为0.变为0调用lambda不再递减。返回一个bool，指出变量是否为0

#include <iostream>


using namespace std;
void mutableLambda (void){
    int i = 10;
    auto f1 = [&i] ()mutable ->bool { if(i >0){i--;return false;} else
        return true;};
    for (int j =0; j< 11; ++j){
        cout << f1 () << " ";
    }
    cout << endl;
}


int main(int argc, char *argv[]) {
    mutableLambda();
    return 0;


}
