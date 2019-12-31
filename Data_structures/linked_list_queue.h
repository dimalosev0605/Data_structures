#ifndef LINKED_LIST_QUEUE_H
#define LINKED_LIST_QUEUE_H

#include <iostream>

template<typename T>
class Linked_list_queue
{
    struct Node
    {
        T item;
        Node* next;
        Node(const T& m_item)
            : item {m_item},
              next {nullptr}
        {}
    };
    Node* head = nullptr;
    Node* tail;
    int m_size = 0;

private:
    void clear() {
        Node* t;
        while(head) {
            t = head->next;
            delete head;
            head = t;
        }
    }
    void copy_from(const Linked_list_queue& rhs) {
        Node* t = rhs.head;
        while(t) {
            this->push(t->item);
            t = t->next;
        }
        m_size = rhs.m_size;
    }
    void move_from(Linked_list_queue& rhs) {
        head = rhs.head;
        tail = rhs.tail;
        m_size = rhs.m_size;
        rhs.head = nullptr;
        rhs.m_size = 0;
    }

public:
    bool empty() const { return head == nullptr; }
    int size() const { return size; }
    void push(const T& item) {
        Node* t = tail;
        tail = new Node(item);
        if(head == nullptr)
            head = tail;
        else
            t->next = tail;
        ++m_size;
    }
    void pop() {
        Node* t = head->next;
        delete head;
        head = t;
        --m_size;
    }
    T& front() const { return head->item; }
    T& back() const { return tail->item; }
    void print() const {
        Node* t = head;
        std::cout << "#############Queue padding#############\n";
        while(t) {
            std::cout << t->item << ' ';
            t = t->next;
        }
        std::cout << "\n#############Queue padding#############\n";
    }
    Linked_list_queue() = default;
    Linked_list_queue(const Linked_list_queue& rhs) { copy_from(rhs); }
    Linked_list_queue& operator=(const Linked_list_queue& rhs) {
        if(this == & rhs) return *this;
        if(!empty()) { clear(); }
        copy_from(rhs);
        return *this;
    }
    Linked_list_queue(Linked_list_queue&& rhs) {
        move_from(rhs);
    }
    Linked_list_queue& operator=(Linked_list_queue&& rhs) {
        if(this == &rhs) return *this;
        if(!empty()) { clear(); }
        move_from(rhs);
    }
    ~Linked_list_queue() { clear(); }
};


#endif // LINKED_LIST_QUEUE_H
