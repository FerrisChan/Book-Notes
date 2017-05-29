//
// Created by ferris on 17-5-27.
//
// 冒泡的容器vector实现法

#include <iostream>
#include <vector>

using namespace std;

int BubbleSort( vector<int> &arr){
    for (vector<int>::size_type index = 0 ; index <= arr.size();index++){
        // 循环size次，
        auto item= arr.begin();  // 迭代器item指向第一个元素
        auto item1= arr.begin(); // item1指向item的下一个元素
        int temp;
        ++ item1;

        for (;item1 != arr.end();++item,++item1){
            // 一次循环比较所有的元素

            if ((*item) > (*item1)){
                // 将重的泡泡置底部
                temp = *item;
                *item = *item1;
                *item1 = temp;}
        }


    }
}
int main() {
    vector<int> vect;  // 输入的容器，int型
    int num;           // vect的元素
    while (cin>> num){
        // 输入数字继续，输入字母就将缓冲区的列表push进vector
        vect.push_back(num);
    }
    BubbleSort( vect);
    for (auto iter = vect.begin();iter != vect.end();++iter)
    {
        cout << *iter << " ";
    }
    cout<<endl;
    return 0;


}