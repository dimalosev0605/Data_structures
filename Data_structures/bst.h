#ifndef BST_H
#define BST_H

#include <iostream>
#include <linked_list_queue.h>

template<typename T>
class Bst
{
    struct Node {
        T item;
        Node* l;
        Node* r;
        Node() = default;
        Node(const T& item)
            : item {item},
              l {nullptr},
              r {nullptr}
        {}
    };
    Node* head = nullptr;
    size_t sz = 0;

    void insert_r(Node*& h, const T& item) {
        if(h == nullptr) { h = new Node(item); return; }
        if(item < h->item)
            insert_r(h->l, item);
        else
            insert_r(h->r, item);
    }
    void print_r(Node* h) const {
        if(h == nullptr) return;
        print_r(h->l);
        std::cout << h->item << ' ';
        print_r(h->r);
    }
    void clear_r(Node*& h) {
        if(h == nullptr) return;
        clear_r(h->l);
        clear_r(h->r);
        delete h;
    }
    void rot_r(Node*& h) {
        Node* x = h->l;
        h->l = x->r;
        x->r = h;
        h = x;
    }
    void rot_l(Node*& h) {
        Node* x = h->r;
        h->r = x->l;
        x->l = h;
        h = x;
    }
    void insert_in_root_r(Node*& h, const T& item) {
        if(h == nullptr) { h = new Node(item); return; }
        if(item < h->item) {
            insert_in_root_r(h->l, item);
            rot_r(h);
        }
        else {
            insert_in_root_r(h->r, item);
            rot_l(h);
        }
    }
    T find_r(Node*& h, const T& item) {
        if(h == nullptr) return T();
        if(h ->item == item) return h->item;
        if(item < h->item)
            find_r(h->l, item);
        else
            find_r(h->r, item);
    }
    Node* copy_r(Node* h) {
        if(h == nullptr) return nullptr;
        Node* t = new Node;
        t->item = h->item;
        t->l = copy_r(h->l);
        t->r = copy_r(h->r);
        return t;
    }
    void copy_from(const Bst& rhs) {
        head = copy_r(rhs.head);
    }
    void move_from(Bst& rhs) {
        head = rhs.head;
        sz = rhs.sz;
        rhs.head = nullptr;
        rhs.sz = 0;
    }

public:
    // Tree traversals:
    // a) inorder - l, root, r - поперечный
    // b) preorder - root, l, r - прямой
    // c) postorder - l, r, root - обратный

    Bst() = default;
    Bst(const Bst& rhs) { copy_from(rhs); }
    Bst& operator=(const Bst& rhs) {
        if(this == &rhs) return *this;
        if(sz) clear_non_recursive();
        copy_from(rhs);
        return *this;
    }
    Bst(Bst&& rhs) { move_from(rhs); }
    Bst& operator=(Bst&& rhs) {
        if(this == &rhs) return *this;
        if(sz) clear_non_recursive();
        move_from(rhs);
        return *this;
    }
    ~Bst() { clear_non_recursive(); }
    size_t size() const { return sz; }
    bool empty() const { return sz == 0; }
    void insert(const T& item) {
        insert_r(head, item);
        ++sz;
    }
    void insert_in_root(const T& item) {
        insert_in_root_r(head, item);
        ++sz;
    }
    void print() const {
        std::cout << "###############Tree padding###############\n";
        print_r(head);
        std::cout << "\n###############Tree padding###############\n";
    }
    void clear_non_recursive() {
        if(head == nullptr) return;
        Linked_list_queue<Node*> q;
        q.push(head);
        while(!q.empty()) {
            Node* n = q.front();
            q.pop();
            if(n->l) q.push(n->l);
            if(n->r) q.push(n->r);
            delete n;
        }
        head = nullptr;
        sz = 0;
    }
    void clear_recursively() {
        clear_r(head);
        head = nullptr;
        sz = 0;
    }
    T find(const T& item) {
        return find_r(head, item);
    }
};

#endif // BST_H
