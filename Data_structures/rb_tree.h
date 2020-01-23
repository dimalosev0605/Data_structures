#ifndef RB_TREE_H
#define RB_TREE_H

#include <iostream>
#include <linked_list_queue.h>

namespace Data_structures {

template<typename T>
class RB_Tree
{
    enum class Color: int { black = 0, red = 1 };
    struct Node {
        T item;
        Node* l;
        Node* r;
        Color color;
        Node() = default;
        Node(const T& item)
            : item {item},
              l {nullptr},
              r {nullptr},
              color {Color::red}
        {}
    };
    Node* head = nullptr;
    size_t sz = 0;

private:
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
    int get_color(Node* n) {
        if(n == nullptr) return static_cast<int>(Color::black);
        return static_cast<int>(n->color);
    }
    void rb_insert(Node*& h, const T& item, Color color) {
        if(h == nullptr) { h = new Node(item); return; }
        if(get_color(h->l) && get_color(h->r)) {
            h->color = Color::red;
            h->l->color = Color::black;
            h->r->color = Color::black;
        }
        if(item < h->item) {
            rb_insert(h->l, item, Color::black);
            if(get_color(h) && get_color(h->l) && static_cast<int>(color)) rot_r(h);
            if(get_color(h->l) && get_color(h->l->l)) {
                rot_r(h);
                h->color = Color::black;
                h->r->color = Color::red;
            }
        }
        else {
            rb_insert(h->r, item, Color::red);
            if(get_color(h) && get_color(h->r) && !static_cast<int>(color)) rot_l(h);
            if(get_color(h->r) && get_color(h->r->r)) {
                rot_l(h);
                h->color = Color::black;
                h->l->color = Color::red;
            }
        }
    }
    T find_r(const Node* h, const T& item) const {
        if(h == nullptr) return T{};
        if(item == h->item) return h->item;
        if(item < h->item)
            return find_r(h->l, item);
        else
            return find_r(h->r, item);
    }
    void print_r(Node* h) const {
        if(h == nullptr) return;
        print_r(h->l);
        std::cout << h->item << ' ';
        print_r(h->r);
    }
    Node* copy_r(Node* h) {
        if(h == nullptr) return nullptr;
        Node* t = new Node;
        t->item = h->item;
        t->color = h->color;
        t->l = copy_r(h->l);
        t->r = copy_r(h->r);
        return t;
    }
    void copy_from(const RB_Tree& rhs) {
        head = copy_r(rhs.head);
        sz = rhs.sz;
    }
    void move_from(RB_Tree& rhs) {
        head = rhs.head;
        sz = rhs.sz;
        rhs.head = nullptr;
        sz = 0;
    }

public:
    RB_Tree() = default;
    RB_Tree(const RB_Tree& rhs) { copy_from(rhs); }
    RB_Tree& operator=(const RB_Tree& rhs) {
        if(this == &rhs) return *this;
        if(sz) clear();
        copy_from(rhs);
        return *this;
    }
    RB_Tree(RB_Tree&& rhs) { move_from(rhs); }
    RB_Tree& operator=(RB_Tree&& rhs) {
        if(this == &rhs) return *this;
        if(sz) clear();
        move_from(rhs);
        return *this;
    }
    ~RB_Tree() { clear(); }
    size_t size() const { return sz; }
    bool empty() const { return sz == 0; }
    void insert(const T& item) {
        rb_insert(head, item, Color::black);
        head->color = Color::black;
        ++sz;
    }
    T find(const T& item) const {
        return find_r(head, item);
    }
    void print() const {
        std::cout << "###########RB_Tree padding###########\n";
        print_r(head);
        std::cout << "\n###########RB_Tree padding###########\n";
    }
    int height() const {
        if(head == nullptr) return 0;
        Linked_list_queue<Node*> q;
        q.push(head);
        int height = 0;
        while(1) {
            int node_count = q.size();
            if(node_count == 0) return height - 1;
            ++height;
            while(node_count > 0) {
                Node* node = q.front();
                q.pop();
                if(node->l != nullptr) q.push(node->l);
                if(node->r != nullptr) q.push(node->r);
                --node_count;
            }
        }
    }
    void clear() {
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
};

}


#endif // RB_TREE_H
