#include "my_string.h"

My_string::My_string()
    : sz {0},
      ptr {ch}
{
    ch[0] = 0;
}

My_string::My_string(const char *p)
    : sz {strlen(p)},
      ptr {sz <= short_max ? ch : new char[sz + 1]},
      space {0}
{
    strcpy(ptr, p);
}

My_string::My_string(const My_string &rhs)
{
    copy_from(rhs);
}

My_string& My_string::operator=(const My_string &rhs)
{
    if(this == &rhs) return *this;
    char* p = short_max < sz ? ptr : nullptr;
    copy_from(rhs);
    delete [] p;
    return *this;
}

My_string::My_string(My_string &&rhs)
{
    move_from(rhs);
}

My_string& My_string::operator=(My_string &&rhs)
{
    if(this == &rhs) return *this;
    if(short_max < sz) delete [] ptr;
    move_from(rhs);
    return *this;
}

My_string::~My_string()
{
    if(sz > short_max) delete [] ptr;
}

const char* My_string::c_str() const
{
    return ptr;
}

size_t My_string::size() const
{
    return sz;
}

size_t My_string::capacity() const
{
    return sz <= short_max ? short_max : space + sz;
}

bool My_string::empty() const
{
    return sz == 0;
}

void My_string::clear()
{
    if(sz > short_max) delete [] ptr;
    sz = 0;
    ch[0] = 0;
    ptr = ch;
}

char& My_string::operator[](size_t n)
{
    return ptr[n];
}

const char& My_string::operator[](size_t n) const
{
    return ptr[n];
}

char& My_string::at(size_t n)
{
    check(n);
    return ptr[n];
}

const char& My_string::at(size_t n) const
{
    check(n);
    return ptr[n];
}

char& My_string::back()
{
    return ptr[sz - 1];
}

const char& My_string::back() const
{
    return ptr[sz - 1];
}

char& My_string::front()
{
    return ptr[0];
}

const char& My_string::front() const
{
    return ptr[0];
}

void My_string::push_back(const char c)
{
    add_char(c);
}

void My_string::pop_back()
{
    ptr[--sz] = 0;
}

My_string& My_string::operator+=(char c)
{
    add_char(c);
    return *this;
}

void My_string::copy_from(const My_string &rhs)
{
    if(rhs.sz <= short_max) {
        memcpy(this, &rhs, sizeof(rhs));
        ptr = ch;
    }
    else {
        ptr = expand(rhs.ptr, rhs.sz + 1);
        sz = rhs.sz;
        space = 0;
    }
}

void My_string::move_from(My_string &rhs)
{
    if(rhs.sz <= short_max) {
        memcpy(this, &rhs, sizeof(rhs));
        ptr = ch;
    }
    else {
        ptr = rhs.ptr;
        sz = rhs.sz;
        space = rhs.space;
        rhs.ptr = rhs.ch;
        rhs.sz = 0;
        rhs.ch[0]= 0;
    }
}

char* My_string::expand(const char *ptr, size_t n)
{
    char* p = new char[n];
    strcpy(p, ptr);
    return p;
}

void My_string::check(size_t n) const
{
    if(n >= sz) throw std::exception("Out of range");
}

void My_string::add_char(const char c)
{
    if(sz == short_max) {
        size_t n = sz + sz + 2;
        ptr = expand(ptr, n);
        space = n - sz - 2;
    }
    else if(short_max < sz) {
        if(space == 0) {
            size_t n = sz + sz + 2;
            char* p = expand(ptr, n);
            delete [] ptr;
            ptr = p;
            space = n - sz - 2;
        }
        else {
            --space;
        }
    }
    ptr[sz] = c;
    ptr[++sz] = 0;
}

std::ostream &operator<<(std::ostream &os, const My_string &str)
{
    return os << str.c_str();
}

My_string operator+(const My_string &lhs, const My_string &rhs)
{
    My_string str;
    if(lhs.sz + rhs.sz > My_string::short_max) {
        str.ptr = new char[lhs.sz + rhs.sz + 1];
        str.sz = lhs.sz + rhs.sz;
        str.space = 0;
        strcpy(str.ptr, lhs.ptr);
        strcpy(str.ptr + lhs.sz, rhs.ptr);
    }
    else {
        strcpy(str.ptr, lhs.ptr);
        strcpy(str.ptr + lhs.sz, rhs.ptr);
        str.sz = lhs.sz + rhs.sz;
    }
    return str;
}
