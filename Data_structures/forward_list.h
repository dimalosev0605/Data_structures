#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include <iostream>

namespace Data_structures {

template<typename T>
class Forward_list
{
    struct Node
    {
        T item;
        Node* next;
        Node() = default;
        Node(const T& m_item, Node* m_next)
            : item {m_item},
              next{m_next}
        {}
    };
    Node* head = nullptr;

private:
    void copy_from(const Forward_list& rhs) {
        size_t size = rhs.size();
        if(size == 1) {
            head = new Node(rhs.head->item, head);
            return;
        }
        if(size == 0) return;
        Node** nl = new Node*[size];
        Node* t = rhs.head;
        int i = 0;
        while(t) {
            nl[i++] = new Node(t->item, nullptr);
            t = t->next;
        }
        for(int j = i - 1; j > 1; --j) {
            nl[j - 1]->next = nl[j];
        }
        head = nl[0];
        head->next = nl[1];
        delete [] nl;
    }
    void move_from(Forward_list& rhs) {
        head = rhs.head;
        rhs.head = nullptr;
    }

public:
    bool empty() const { return head == nullptr; }
    T& front() const { return head->item; }
    size_t size() const {
        size_t size {0};
        Node* t = head;
        while(t) {
            t = t->next;
            ++size;
        }
        return size;
    }
    void push_front(const T& item) { head = new Node(item, head); }
    void pop() {
        Node* t = head->next;
        delete head;
        head = t;
    }
    void clear() {
        Node* t;
        while(head) {
            t = head->next;
            delete head;
            head = t;
        }
    }
    void remove(const T& item) {
        if(head == nullptr) return;
        Node* t = head;
        Node* prev = head;
        while(t->item != item) {
            prev = t;
            t = t->next;
            if(t == nullptr) return;
        }
        if(t == head) {
            delete head;
            head = nullptr;
            return;
        }
        Node* next = t->next;
        delete t;
        prev->next = next;
    }
    void remove_if(bool (*op)(const T& elem)) {
        Node* curr = head;
        Node* prev = head;
        while(curr) {
            if(op(curr->item)) {
                if(curr == head) {
                    Node* temp = head->next;
                    delete head;
                    head = curr = prev = temp;
                    continue;
                }
                Node* n = curr->next;
                delete curr;
                prev->next = n;
                curr = n;
            }
            else {
                prev = curr;
                curr = curr->next;
            }
        }
    }
    void reverse() {
        Node* t;
        Node* y = head;
        Node* r = nullptr;
        while(y) {
            t = y->next;
            y->next = r;
            r = y;
            y = t;
        }
        head = r;
    }
    void insert_after(const T& where, const T& value) { // where - node after which new node will be inserted
        if(head == nullptr) return;
        Node* t = head;
        while(t->item != where) {
            t = t->next;
            if (!t) return;
        }
        Node* t2 = new Node(value, t->next);
        t->next = t2;
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
    void set_max_in_head() {
        Node* max = head;
        Node* curr = head->next;
        Node* prev_max = head;
        Node* prev = head;
        while(curr) {
            if(curr->item > max->item) {
                prev_max = prev;
                max = curr;
            }
            prev = curr;
            curr = curr->next;
        }
        if(max == head) return;
        prev_max->next = max->next;
        Node* temp = head;
        head = max;
        head->next = temp;
    }

    void split(Node* l, Node** front, Node** back) {
        Node* fast;
        Node* slow;
        slow = l;
        fast = l->next;
        while(fast) {
            fast = fast->next;
            while(fast) {
                slow = slow->next;
                fast = fast->next;
            }
        }
        *front = l;
        *back = slow->next;
        slow->next = nullptr;
    }

    Node* sorted_merge(Node* a, Node* b) {
        Node* res = nullptr;
        if(a == nullptr)
            return b;
        if(b == nullptr)
            return a;
        if(a->item <= b->item) {
            res = a;
            res->next = sorted_merge(a->next, b);
        }
        else {
            res = b;
            res->next = sorted_merge(a, b->next);
        }
        return res;
    }

    void sort() {
        sort_r(&head);
    }

    void sort_r(Node** h) {
        Node* head = *h;
        Node* a;
        Node* b;
        if(!head || !head->next) return;
        split(head, &a, &b);
        sort_r(&a);
        sort_r(&b);
        *h = sorted_merge(a, b);
    }

    Forward_list() = default;
    Forward_list(const Forward_list& rhs) { copy_from(rhs); }
    Forward_list& operator=(const Forward_list& rhs) {
        if(this == &rhs) return *this;
        clear();
        copy_from(rhs);
        return *this;
    }
    Forward_list(Forward_list&& rhs) { move_from(rhs); }
    Forward_list& operator=(Forward_list&& rhs) {
        if(this == &rhs) return *this;
        clear();
        move_from(rhs);
        return *this;
    }
    ~Forward_list() { clear(); }
};

}

#endif // FORWARD_LIST_H
