//
// Created by ferris on 17-7-23.
// 类模板 Blob.h

#ifndef CODE_CHAP16_BLOB_H
#define CODE_CHAP16_BLOB_H


template<typename T> class Blob
{
public:
    typedef T value_type;
    typedef typename std::vector<T>::size_type size_type;

    // 构造函数
    Blob();
    Blob(std::initializer_list<T> il);

    // 元素操作
    size_type size() const { return data->size(); }
    bool      empty() const { return data->empty(); }

    void push_back(const T& t) { data->push_back(t); }
    void push_back(T&& t) { data->push_back(std::move(t)); }
    void pop_back();

    // element access
    T& back();
    T& operator[](size_type i);

    const T& back() const;
    const T& operator [](size_type i) const;

private:
    std::shared_ptr<std::vector<T>> data;
    // throw msg if data[i] isn't valid
    void check(size_type i, const std::string &msg) const;
};


#endif //CODE_CHAP16_BLOB_H
