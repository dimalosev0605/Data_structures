#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

namespace Data_structures {

template<typename T>
class Vector
{
    T* data = nullptr;
    size_t m_size = 0 ;
    size_t m_capacity = 0;

private:
    void expand() {
        T* new_space;
        if(m_size == 0) {
            data = new T[1];
            m_capacity = 1;
            return;
        }
        else {
            new_space = new T[m_size * 2];
        }
        for(size_t i = 0; i < m_size; ++i) new_space[i] = std::move(data[i]);
        delete [] data;
        data = new_space;
        m_capacity = m_size;
    }
    void copy_from(const Vector& rhs) {
        data = new T[rhs.m_size + rhs.m_capacity];
        for(size_t i = 0; i < rhs.m_size; ++i) data[i] = rhs[i];
        m_size = rhs.m_size;
        m_capacity = rhs.m_capacity;
    }
    void move_from(Vector& rhs) {
        memcpy(this, &rhs, sizeof(Vector));
        rhs.data = nullptr;
        rhs.m_size = 0;
        rhs.m_capacity = 0;
    }
    void check(size_t index) const { if(index >= m_size || index < 0) throw std::out_of_range("Out of range error"); }

public:
    size_t size() const { return m_size; }
    size_t capacity() const { return m_capacity; }
    T& back() const { return data[m_size - 1]; }
    T& front() const { return data[0]; }
    bool empty() const { return m_size == 0; }
    T* get_data() const { return data; }
    T& operator[](size_t index) { return data[index]; }
    const T& operator[](size_t index) const { return data[index]; }
    T& at(size_t index) { check(index); return data[index]; }
    const T& at(size_t index) const { check(index); return data[index]; }
    void push_back(const T& item) {
        if(m_capacity) {
            data[m_size++] = item;
            --m_capacity;
        }
        else {
            expand();
            data[m_size++] = item;
            --m_capacity;
        }
    }
    void push_back(T&& item) {
        if(m_capacity) {
            data[m_size++] = std::move(item);
            --m_capacity;
        }
        else {
            expand();
            data[m_size++] = std::move(item);
            --m_capacity;
        }
    }
    void pop_back() {
        data[--m_size].~T();
        ++m_capacity;
    }
    void clear() {
        m_size = 0;
        m_capacity = 0;
        delete [] data;
    }
    void shrink_to_fit() {
        if(m_size == 0) return;
        T* new_space = new T[m_size];
        for(size_t i = 0; i < m_size; ++i) new_space[i] = std::move(data[i]);
        delete [] data;
        data = new_space;
        m_capacity = 0;
    }
    void reserve(size_t n) {
        if(n < m_capacity) return;
        T* new_space = new T[m_size + n];
        for(size_t i = 0; i < m_size; ++i) new_space[i] = std::move(data[i]);
        delete [] data;
        data = new_space;
        m_capacity = n;
    }
    void resize(size_t n) {
        if(n > m_size) {
            T t{};
            for(; n; --n) push_back(t);
        }
        else {
            for(size_t diff = m_size - n; diff; --diff) pop_back();
        }
    }
    void resize(size_t n, const T& elem) {
        if(n > m_size) {
            T t{elem};
            for(; n; --n) push_back(t);
        }
        else {
            for(size_t diff = m_size - n; diff; --diff) pop_back();
        }
    }
    void erase(const T& value) {
        int i = 0;
        for(; i < m_size; ++i) if(data[i] == value) break;
        if(i == m_size) return;
        data[i].~T();
        for(int j = i; j < m_size - 1; ++j) { data[j] = std::move(data[j + 1]); }
        pop_back();
    }
    void insert(size_t where_index, const T& item) {
        push_back(item);
        for(int i = m_size - 1; i != where_index; --i) { swap(data[i], data[i - 1]); }
    }
    void print() const {
        std::cout << "###########Vector padding###########\n";
        for(int i = 0; i < m_size; ++i) {
            std::cout << data[i] << ' ';
        }
        std::cout << "\n###########Vector padding###########\n";
    }
    Vector() = default;
    Vector(const Vector& rhs) { copy_from(rhs); }
    Vector& operator=(const Vector& rhs) {
        if(this == &rhs) return *this;
        if(m_size) clear();
        copy_from(rhs);
        return *this;
    }
    Vector(Vector&& rhs) { move_from(rhs); }
    Vector& operator=(Vector&& rhs) {
        if(this == &rhs) return *this;
        if(m_size) clear();
        move_from(rhs);
        return *this;
    }
    ~Vector() { clear();}
};

}


#endif // VECTOR_H
