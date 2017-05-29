//
// Created by ferris on 17-5-26.
//
//冒泡排序
//
#include <iostream>
using namespace std;

void BubbleSort(int *array,int arrLength){
    int time,index,temp;
    for(time=0;time!=arrLength;++time){
        for (index=0;index!=arrLength-time;++index){
            if(array[index]>array[index+1]){
                temp = array[index];
                array[index] = array[index+1];
                array[index+1] = temp;

            }
        }
    }

}


int main() {
    int array[10],arrLength;
    cout<<"Input ten number:";
    for(int i=0;i<10;i++)
        cin>>array[i];
    cout<<endl;
    arrLength = sizeof(array)/sizeof(array[0]);  //计算数组长度
    BubbleSort(array,arrLength);
    for(int i=0;i<10;i++)
        cout<<array[i]<<" ";
    cout<<endl;
    return 0;


}