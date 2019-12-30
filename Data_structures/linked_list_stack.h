#ifndef LINKED_LIST_STACK_H
#define LINKED_LIST_STACK_H

#include <iostream>

template<typename T>
class Linked_list_stack
{
    struct Node
    {
        T item;
        Node* next;
        Node(const T& m_item, Node* m_next)
            : item {m_item},
              next {m_next}
        {}
    };
    Node* head = nullptr;

public:
    bool empty() const { return head == nullptr; }
    void push(const T& item) { head = new Node(item, head); }
    void pop() {
        Node* t = head->next;
        delete head;
        head = t;
    }
    T& top() const { return head->item; }
    int size() const {
        int size {0};
        Node* t = head;
        while(t) {
            ++size;
            t = t->next;
        }
        return size;
    }
    void print() const {
        Node* t = head;
        std::cout << "################Stack padding################\n";
        while(t) {
            std::cout << t->item << ' ';
            t = t->next;
        }
        std::cout << "\n################Stack padding################\n";
    }
    Linked_list_stack() = default;
    ~Linked_list_stack() {
        Node* t = head;
        Node* t2;
        while(t) {
            t2 = t->next;
            delete t;
            t = t2;
        }
    }
};

#endif // LINKED_LIST_STACK_H
