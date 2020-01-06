#ifndef MY_STRING_H
#define MY_STRING_H

#include <cstring>
#include <exception>
#include <iostream>

class My_string
{
    static const int short_max = 15;
    size_t sz;
    char* ptr;
    union {
        size_t space; // extra allocated space.
        char ch[short_max + 1]; // +1 for '\0'.
    };

private:
    void copy_from(const My_string& rhs);
    void move_from(My_string& rhs);
    char* expand(const char* ptr, size_t n);
    void check(size_t n) const;
    void add_char(const char c);

public:
    My_string();
    My_string(const char* p);
    My_string(const My_string& rhs);
    My_string& operator=(const My_string& rhs);
    My_string(My_string&& rhs);
    My_string& operator=(My_string&& rhs);
    ~My_string();
    const char* c_str() const;
    size_t size() const;
    size_t capacity() const;
    bool empty() const;
    void clear();
    char& operator[](size_t n);
    const char& operator[](size_t n) const;
    char& at(size_t n);
    const char& at(size_t n) const;
    char& back();
    const char& back() const;
    char& front();
    const char& front() const;
    void push_back(const char c);
    void pop_back();
    My_string& operator+=(char c);
    friend My_string operator+(const My_string& lhs, const My_string& rhs);
};

std::ostream& operator<<(std::ostream& os, const My_string& str);

#endif // MY_STRING_H
