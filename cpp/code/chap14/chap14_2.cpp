//
// Created by ferris on 17-7-22.
//hap14.2 为Sales_data编写重载的输入、输出、加法和复合赋值运算符

#include "chap14_2.h"

Sales_data::Sales_data(std::istream &is) : Sales_data()
{
    is >> *this;
}


// 定义复合加法运算符
Sales_data& Sales_data::operator+=(const Sales_data &rhs)
{
    units_sold += rhs.units_sold;
    revenue += rhs.revenue;
    return *this;
}

// 定义重载输入运算符，必须含有错误检测
std::istream& operator>>(std::istream &is, Sales_data &item)
{
    double price = 0.0;
    is >> item.bookNo >> item.units_sold >> price;
    if (is)
        item.revenue = price * item.units_sold;
    else
        item = Sales_data();
    return is;
}

// 定义输出运算符，不需要错误检测
std::ostream& operator<<(std::ostream &os, const Sales_data &item)
{
    os << item.isbn() << " " << item.units_sold << " " << item.revenue << " " << item.avg_price();
    return os;
}

// 利用复合加法来定义算术运算符
Sales_data operator+(const Sales_data &lhs, const Sales_data &rhs)
{
    Sales_data sum = lhs;
    sum += rhs;
    return sum;
}