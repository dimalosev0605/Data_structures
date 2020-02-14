#ifndef DEQUE_H
#define DEQUE_H

#include <iostream>
#include <utility>
#include <exception>

template<typename T>
class Deque
{
    static const int first_allocation_size = 3;

    T* mem_begin;
    T* left; // will move in left
    T* right; // will move in right
    T* mem_end;
    int item_count;
    int mem_size;

private:
    void destroy_items() {
        for(T* i = left; i != right; ++i) i->~T();
    }

    void expand() {
        // Imagine that we already have inserted item. It is the cause of magic "+1" in (item_count + 1) expressions.
        T* new_space = static_cast<T*>(malloc(mem_size * 2 * sizeof(T)));
        if(new_space == nullptr) { return; } // or can throw
        T* ptr = new_space + mem_size - (item_count + 1) / 2;
        T* new_left = ptr;
        memcpy(ptr, left, sizeof(T) * item_count);
        free(mem_begin);
        mem_size *= 2;
        mem_begin = new_space;
        left = new_left;
        right = left + (item_count + 1) - 1;
        mem_end = mem_begin + mem_size;
    }

    void check_for_expand() {
        if(left == mem_begin || right == (mem_end - 1)) expand();
    }

public:
    Deque()
        : mem_begin( static_cast<T*>(malloc (first_allocation_size * sizeof(T))) ),
          left(mem_begin + first_allocation_size / 2),
          right(left),
          mem_end(mem_begin + first_allocation_size),
          item_count(0),
          mem_size(first_allocation_size)
    {}
    Deque(const Deque& rhs)
        : mem_begin( static_cast<T*>(malloc (sizeof(T) * rhs.mem_size)) ),
          left(mem_begin + (rhs.left - rhs.mem_begin)),
          right(left),
          mem_end(mem_begin + rhs.mem_size),
          item_count(rhs.item_count),
          mem_size(rhs.mem_size)
    {
        for(T* i = rhs.left; i != rhs.right; ++i, ++right) { new (right) T(*i); }
    }
    Deque(Deque&& rhs)
        : mem_begin(rhs.mem_begin),
          left(rhs.left),
          right(rhs.right),
          mem_end(rhs.mem_end),
          item_count(rhs.item_count),
          mem_size(rhs.mem_size)
    {
        rhs.mem_begin = nullptr;
        rhs.left = nullptr;
        rhs.right = nullptr;
        rhs.mem_end = nullptr;
        rhs.item_count = 0;
        rhs.mem_size = first_allocation_size;
    }
    Deque& operator=(Deque&& rhs) {
        destroy_items();
        free(mem_begin);

        mem_begin = rhs.mem_begin;
        left = rhs.left;
        right = rhs.right;
        mem_end = rhs.mem_end;
        item_count = rhs.item_count;
        mem_size = rhs.mem_size;

        rhs.mem_begin = nullptr;
        rhs.left = nullptr;
        rhs.right = nullptr;
        rhs.mem_end = nullptr;
        rhs.item_count = 0;
        rhs.mem_size = first_allocation_size;

        return *this;
    }
    Deque& operator=(const Deque& rhs) {
        T* memory = static_cast<T*>(malloc(sizeof(T) * rhs.mem_size));
        if(memory) {
            T* old_memory = mem_begin;
            T* old_left = left;
            T* old_right = right;

            mem_begin = memory;
            left = mem_begin + (rhs.left - rhs.mem_begin);
            right = left;
            mem_end = mem_begin + rhs.mem_size;
            item_count = rhs.item_count;
            mem_size = rhs.mem_size;

            for(T* i = rhs.left; i != rhs.right; ++i, ++right) { new (right) T(*i); }

            for(T* i = old_left; i != old_right; ++i) i->~T();
            free(old_memory);
        }
        return *this;
    }
    ~Deque() { destroy_items(); free(mem_begin); }

    void push_back(const T& item) {
        check_for_expand();
        new (right) T(item);
        ++right;
        ++item_count;
    }
    void push_back(T&& item) {
        check_for_expand();
        new (right) T(std::move(item));
        ++right;
        ++item_count;
    }
    void pop_back() {
        --right;
        right->~T();
        --item_count;
    }
    template<typename... Args>
    void emplace_back(Args&&... args) {
        check_for_expand();
        new (right) T(std::forward<Args>(args)...);
        ++right;
        ++item_count;
    }

    void push_front(const T& item) {
        check_for_expand();
        --left;
        new (left) T(item);
        ++item_count;
    }
    void push_front(T&& item) {
        check_for_expand();
        --left;
        new (left) T(std::move(item));
        ++item_count;
    }
    void pop_front() {
        left->~T();
        ++left;
        --item_count;
    }
    template<typename... Args>
    void emplace_front(Args... args) {
        check_for_expand();
        --left;
        new (left) T(std::forward<Args>(args)...);
        ++item_count;
    }

    T& operator[](int n) { return left[n]; }
    const T& operator[](int n) const { return left[n]; }

    T& front() { return *left; }
    const T& front() const { return *left; }

    T& back() { return *(right - 1); }
    const T& back() const { return *(right - 1); }

    int size() const { return item_count; }
    bool empty() const { return item_count == 0; }

    void print_all_mem() const {
        for(T* i = mem_begin; i != mem_end; ++i) {
            std::cout << *i << ' ';
        }
        std::cout << '\n';
    }
    void print() const {
        for(T* i = left; i != right; ++i) {
            std::cout << *i << ' ';
        }
        std::cout << '\n';
    }
};

#endif // DEQUE_H
