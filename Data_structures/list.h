#ifndef LIST_H
#define LIST_H

#include <iostream>

template<typename T>
class List
{
    struct Node
    {
        T item;
        Node* previous;
        Node* next;
        Node() = default;
        Node(const T& m_item, Node* p, Node* n)
            : item {m_item},
              previous{p},
              next{n}
        {}
    };

    Node* head = nullptr;
    Node* tail = nullptr;
    size_t sz = 0;

private:
    void copy_from(const List& rhs) {
        Node* t = rhs.head;
        while(t) {
            push_back(t->item);
            t = t->next;
        }
        sz = rhs.sz;
    }
    void move_from(List& rhs) {
        head = rhs.head;
        tail = rhs.tail;
        sz = rhs.sz;
        rhs.head = rhs.tail = nullptr;
        rhs.sz = 0;
    }

public:
    List() = default;
    List(const List& rhs) { copy_from(rhs); }
    List& operator=(const List& rhs) {
        if(this == &rhs) return *this;
        if(sz) clear();
        copy_from(rhs);
        return *this;
    }
    List(List&& rhs) { move_from(rhs); }
    List& operator=(List&& rhs) {
        if(this == &rhs) return *this;
        if(sz) clear();
        move_from(rhs);
        return *this;
    }
    ~List() { clear(); }
    size_t size() const { return sz; }
    bool empty() const { return sz == 0; }
    T& back() const { return tail->item; }
    T& front() const { return head->item; }
    void push_back(const T& item) {
        if(sz == 0) {
            head = new Node(item, nullptr, nullptr);
            tail = head;
            ++sz;
            return;
        }
        Node* t = tail;
        tail = new Node(item, t, nullptr);
        t->next = tail;
        ++sz;
    }
    void pop_back() {
        if(sz == 1) {
            delete head;
            head = tail = nullptr;
            --sz;
            return;
        }
        Node* t = tail->previous;
        delete tail;
        tail = t;
        tail->next = nullptr;
        --sz;
    }
    void clear() {
        Node* t;
        while(head) {
            t = head->next;
            delete head;
            head = t;
        }
        sz = 0;
    }
    void print_forward() const {
        Node* t = head;
        while(t) {
            std::cout << t->item << ' ';
            t = t->next;
        }
        std::cout << '\n';
    }
    void print_reverse() const {
        Node* t = tail;
        while(t) {
            std::cout << t->item << ' ';
            t = t->previous;
        }
        std::cout << '\n';
    }
    void push_front(const T& item) {
        if(sz == 0) {
            head = new Node(item, nullptr, nullptr);
            tail = head;
            ++sz;
            return;
        }
        Node* t = head;
        head = new Node(item, nullptr, t);
        t->previous = head;
        ++sz;
    }
    void pop_front() {
        if(sz == 1) {
            delete head;
            head = tail = nullptr;
            --sz;
            return;
        }
        Node* t = head;
        head = head->next;
        head->previous = nullptr;
        delete t;
        --sz;
    }
    void resize(size_t n) {
        if(n > sz) {
            T t{};
            size_t diff = n - sz;
            while(diff--) push_back(t);
        }
        else {
            size_t diff = sz - n;
            while(diff--) pop_back();
            sz = n;
        }
    }
    void resize(size_t n, const T& item) {
        if(n > sz) {
            T t{item};
            size_t diff = n - sz;
            while(diff--) push_back(t);
        }
        else {
            size_t diff = sz - n;
            while(diff--) pop_back();
            sz = n;
        }
    }
    void reverse() {
        Node* t = head;
        while(t) {
            std::swap(t->next, t->previous);
            t = t->previous;
        }
        std::swap(head, tail);
    }
    void remove(const T& item) {
        Node* t = head;
        while(t) {
            if(t->item == item) {
                if(t == head) {
                    Node* temp = head->next;
                    delete head;
                    head = temp;
                    head->previous = nullptr;
                    t = head;
                    --sz;
                    continue;
                }
                else if(t == tail) {
                    Node* temp = tail->previous;
                    delete tail;
                    tail = temp;
                    tail->next = nullptr;
                    --sz;
                    return;
                }
                else {
                    Node* p = t->previous;
                    Node* n = t->next;
                    delete t;
                    p->next = n;
                    n->previous = p;
                    --sz;
                    t = n;
                    continue;
                }
            }
            t = t->next;
        }
    }
};

#endif // LIST_H
