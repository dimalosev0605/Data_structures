#ifndef LINKED_LIST_STACK_H
#define LINKED_LIST_STACK_H

#include <iostream>

namespace Data_structures {

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
    void clear() {
        Node* t = head;
        Node* t2;
        while(t) {
            t2 = t->next;
            delete t;
            t = t2;
        }
    }
    void copy_from(const Linked_list_stack& rhs) {
        int size = rhs.size();
        if(size == 1) {
            head = new Node(rhs.head->item, head);
            return;
        }
        if(size == 0) return;
        Node** ns = new Node*[size];
        Node* t = rhs.head;
        int i = 0;
        while(t) {
            ns[i++] = new Node(t->item, nullptr);
            t = t->next;
        }
        for(int j = i - 1; j > 1; --j) {
            ns[j - 1]->next = ns[j];
        }
        head = ns[0];
        head->next = ns[1];
        delete [] ns;
    }
    void move_from(Linked_list_stack& rhs) {
        head = rhs.head;
        rhs.head = nullptr;
    }
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
    Linked_list_stack(const Linked_list_stack& rhs) { copy_from(rhs); }
    Linked_list_stack& operator=(const Linked_list_stack& rhs) {
        if(this == &rhs) return *this;
        if(head) { clear(); }
        copy_from(rhs);
        return *this;
    }
    Linked_list_stack(Linked_list_stack&& rhs) { move_from(rhs); }
    Linked_list_stack& operator=(Linked_list_stack&& rhs) {
        if(this == &rhs) return *this;
        clear();
        move_from(rhs);
        return *this;
    }
    ~Linked_list_stack() { clear(); }
};

}


#endif // LINKED_LIST_STACK_H
