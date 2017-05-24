//
// Created by ferris on 17-5-23.
// 构建自己的类 Person

#ifndef CODE_PERSON_H
#define CODE_PERSON_H

#endif //CODE_PERSON_H

#include <iostream>
#include <string>
using namespace std;
class Person {

private:
    string strName;   // 姓名
    string strAddress;  // 地址

    // practise 7.5 返回姓名和地址
public:
    string getName() const { return  strName;}
    string getAddress() const { return strAddress;}

    // 构造函数 7.15
    Person () = default;
    Person( string &name, string &add):strName(name), strAddress(add) { }


    //7.9 读取和打印对象
friend istream &read (istream &is, Person &per){
        is >> per.strName>> per.strAddress;
        return is;
        }
friend ostream &print (ostream &os,const Person &per) {
        os << per.getAddress()<<per.getName();
        return os;
        }
};