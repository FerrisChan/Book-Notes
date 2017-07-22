//
// Created by ferris on 17-7-22.
// chap14.2 为Sales_data编写重载的输入、输出、加法和复合赋值运算符


#ifndef CODE_CHAP14_2_H
#define CODE_CHAP14_2_H

#include <string>
#include <iostream>

class Sales_data
{
    friend std::istream& operator>>(std::istream&, Sales_data&); // 输入，不是类成员
    friend std::ostream& operator<<(std::ostream&, const Sales_data&); // 输出，不是类成员
    friend Sales_data operator+(const Sales_data&, const Sales_data&); // 加法，非类成员函数

public:
    Sales_data(const std::string &s, unsigned n, double p) :bookNo(s), units_sold(n), revenue(n*p) {}
    Sales_data() : Sales_data("", 0, 0.0f) {}
    Sales_data(const std::string &s) : Sales_data(s, 0, 0.0f) {}
    Sales_data(std::istream &is);

    Sales_data& operator+=(const Sales_data&); // compound-assignment 复合运算符，类成员
    std::string isbn() const { return bookNo; }

private:
    inline double avg_price() const;

    std::string bookNo;
    unsigned units_sold = 0;
    double revenue = 0.0;
};

std::istream& operator>>(std::istream&, Sales_data&);
std::ostream& operator<<(std::ostream&, const Sales_data&);
Sales_data operator+(const Sales_data&, const Sales_data&);

inline double Sales_data::avg_price() const
{
    return units_sold ? revenue / units_sold : 0;
}

#endif //CODE_CHAP14_2_H
