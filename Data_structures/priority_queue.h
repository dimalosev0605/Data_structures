#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <utility>

namespace Data_structures {

template<typename T>
class Priority_queue
{
    T* data;
    size_t items_count;
    size_t mem_size;
    static const int start_size = 3;

private:
    void fix_up(T* a, size_t k) {
        while(k > 1 && a[k >> 1] < a[k]) {
            swap(a[k], a[k >> 1]);
            k >>= 1;
        }
    }
    void fix_down(T* a, size_t k, size_t n) {
        while(k << 1 <= n)
        {
            size_t j = k << 1;
            if(j < n && a[j] < a[j + 1]) ++j;
            if(!(a[k] < a[j])) break;
            swap(a[k], a[j]);
            k = j;
        }
    }
    void expand() {
        T* new_space = new T[items_count << 1];
        for(size_t i = 0; i < items_count + 1; ++i) new_space[i] = std::move(data[i]);
        delete [] data;
        data = new_space;
        mem_size = items_count - 1;
    }
    void copy_from(const Priority_queue& rhs) {
        data = new T[rhs.mem_size + rhs.items_count];
        for(size_t i = 0; i < rhs.items_count; ++i) data[i] = rhs.data[i];
        items_count = rhs.items_count;
        mem_size = rhs.mem_size;
    }
    void move_from(Priority_queue& rhs) {
        data = rhs.data;
        items_count = rhs.items_count;
        mem_size = rhs.mem_size;

        rhs.data = new T[start_size];
        rhs.mem_size = start_size - 1;
        rhs.items_count = 0;
    }

public:
    Priority_queue()
        : data {new T[start_size]},
          items_count {0},
          mem_size {start_size - 1}
    {}
    Priority_queue(const Priority_queue& rhs) { copy_from(rhs); }
    Priority_queue& operator=(const Priority_queue* rhs) {
        if(this == &rhs) return *this;
        delete [] data;
        copy_from(rhs);
        return *this;
    }
    Priority_queue(Priority_queue&& rhs) { move_from(rhs); }
    Priority_queue& operator=(Priority_queue&& rhs) {
        if(this == &rhs) return *this;
        delete [] data;
        move_from(rhs);
        return *this;
    }
    ~Priority_queue() { delete [] data; }
    bool empty() const { return items_count == 0; }
    size_t size() const { return items_count; }
    void insert(const T& item) {
        if(!mem_size) expand();
        data[++items_count] = item;
        --mem_size;
        fix_up(data, items_count);
    }
    T& front() const { return data[1]; }
    void pop() {
        swap(data[1], data[items_count]);
        fix_down(data, 1, items_count - 1);
        ++mem_size;
        data[items_count--].~T();
    }
};

}


#endif // PRIORITY_QUEUE_H
