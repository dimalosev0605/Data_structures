#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <utility>

/*

 Hasher - a unary function object type that takes an object of the same type as the elements as argument
 and returns a unique value of type size_t based on it,
 i.e. for Hasher you must implement: size_t operator()(const T&) const {...}.

 Pred - a binary predicate that takes two arguments of the same type as the elements and returns a bool.
 The expression pred(obj_1, obj_2), where pred is an object of this type and obj_1 and obj_2 are key values,
 shall return true if obj_1 is to be considered equivalent to obj_2.
 i.e. for Pred you must implement: bool operator()(const T&, const T&) const {...}.

 You can't use values created by default constructor of type T in container.
 Also you can't use built-in types(int, float...) in this container.

 You can't set max_load_factor greater than 0.9f.
 After using move constructor or move assign operator you can no longer use moved container.

*/

namespace Data_structures {

template<typename T, typename Hasher, typename Pred>
class Hash_table
{
    size_t m = beginning_table_size;
    size_t sz = 0;
    T* st;
    Hasher hasher;
    Pred pred;
    float maximum_load_factor = default_max_load_factor;

    static const size_t probing_step = 7;
    static const size_t beginning_table_size = 113;
    constexpr static const float default_max_load_factor = 0.5f;

    size_t hash(size_t hasher_value) const { return (hasher_value % probing_step) + 1; }
    size_t calc_index(size_t hasher_value) const { return hasher_value % m; }
    void expand() {
        T* t = st;
        st = new T[m * 2];
        T temp{};
        sz = 0;
        size_t old_m = m;
        m = m * 2;
        for(size_t i = 0; i < old_m; ++i) {
            if(!pred.operator()(t[i], temp))
                insert(t[i]);
        }
        delete [] t;
    }
    void copy_from(const Hash_table& rhs) {
        m = rhs.m;
        sz = rhs.sz;
        hasher = rhs.hasher;
        pred = rhs.pred;
        maximum_load_factor = rhs.maximum_load_factor;
        st = new T[m];
        T t{};
        for(size_t i = 0; i < m; ++i) {
            if(!pred.operator()(rhs.st[i], t))
                st[i] = rhs.st[i];
        }
    }
    void move_from(Hash_table& rhs) {
        m = rhs.m;
        sz = rhs.sz;
        hasher = std::move(rhs.hasher);
        pred = std::move(rhs.pred);
        maximum_load_factor = rhs.maximum_load_factor;
        st = rhs.st;
        rhs.st = nullptr;
    }
public:
    Hash_table()
        : st(new T[m]),
          hasher(Hasher()),
          pred(Pred())
    {}
    Hash_table(const Hash_table& rhs) { copy_from(rhs); }
    Hash_table& operator=(const Hash_table& rhs) {
        if(this == &rhs) return *this;
        delete [] st;
        copy_from(rhs);
        return *this;
    }
    Hash_table(Hash_table&& rhs) { move_from(rhs); }
    Hash_table& operator==(Hash_table&& rhs) {
        if(this == &rhs) return *this;
        delete [] st;
        move_from(rhs);
        return *this;
    }
    ~Hash_table() { delete [] st; }
    size_t size() const { return sz; }
    bool empty() const { return sz == 0; }
    float load_factor() const { return float(sz) / m; }
    float max_load_factor() const { return maximum_load_factor; }
    void max_load_factor(float v) {
        if(v > 0.9f) return;
        maximum_load_factor = v;
    }
    void clear() {
        sz = 0;
        T t{};
        for(size_t i = 0; i < m; ++i) {
            if(!pred.operator()(st[i], t)) st[i] = t;
        }
    }
    void insert(const T& item) {
        if(load_factor() > maximum_load_factor) expand();
        size_t hasher_value = hasher.operator()(item);
        size_t i = calc_index(hasher_value);
        size_t step = hash(hasher_value);
        T t{};
        while(!pred.operator()(st[i], t)) {
            if(pred.operator()(st[i], item)) return;
            i = (i + step) % m;
        }
        st[i] = item;
        ++sz;
    }
    T searh(const T& item) {
        size_t hasher_value = hasher.operator()(item);
        size_t i = calc_index(hasher_value);
        size_t step = hash(hasher_value);
        T t{};
        while(!pred.operator()(st[i], item) && !pred.operator()(t, st[i])) {
            i = (i + step) % m;
        }
        return st[i];
    }
    void remove(const T& item) {
        size_t hasher_value = hasher.operator()(item);
        size_t i = calc_index(hasher_value);
        size_t step = hash(hasher_value);
        T t{};
        while(!pred.operator()(st[i], t)) {
            if(pred.operator()(st[i], item))
                break;
            else
                i = (i + step) % m;
        }
        if(pred.operator()(t, st[i])) return;
        st[i] = t;
        --sz;
        for(size_t j = i + step; !pred.operator()(st[j], t); j = (j + step) % m, --sz) {
            T v = std::move(st[j]);
            st[j] = t;
            insert(v);
        }
    }
};

}


#endif // HASH_TABLE_H
