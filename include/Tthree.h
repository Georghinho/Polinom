#pragma once
#include "polinom.h"

template <class Tkey, class Tvalue>
class RBTree {
public:
    enum Color { RED, BLACK };
    struct node {
        Tkey key;
        Tvalue value;
        node* left, * right, * parent;
        Color color;
        bool fict;              // true для NIL-узлов

        node() {
            left = nullptr;
            fict = true;
            right = nullptr;
            color = BLACK;
            parent = nullptr;
        }
        node(Tkey k, Tvalue val, Color c = BLACK, bool fict = false, node* parent = nullptr) :
            key(k), value(val), color(c), parent(parent) {
            this->fict = fict;
            left = right = nullptr;
        }
    };
    node* root;
    size_t sz;

    void createFict(node* x) {
        x->left = new node();
        x->left->parent = x;
        x->right = new node();
        x->right->parent = x;
    }

    void Destr(node* x) {
        if (!x) return;
        if (x->left && !x->left->fict) Destr(x->left);
        if (x->right && !x->right->fict) Destr(x->right);
        delete x;
    }

    void copy(node* first, node* second) {
        if (!second || second->fict) return;

        createFict(first);
        first->color = second->color;
        first->key = second->key;
        first->value = second->value;
        first->fict = second->fict;

        if (second->left && !second->left->fict) {
            copy(first->left, second->left);
            first->left->parent = first;
        }
        if (second->right && !second->right->fict) {
            copy(first->right, second->right);
            first->right->parent = first;
        }
    }

public:
    class iterator {
    public:
        node* ptr;

        iterator(node* p = nullptr) : ptr(p) {}

        iterator operator++() {
            if (!ptr) return *this;

            if (ptr->right && !ptr->right->fict) {
                ptr = ptr->right;
                while (ptr->left && !ptr->left->fict)
                    ptr = ptr->left;
                return *this;
            }

            node* par = ptr->parent;
            while (par && ptr == par->right) {
                ptr = par;
                par = par->parent;
            }
            ptr = par;
            return *this;
        }

        bool operator==(const iterator& it) const {
            return ptr == it.ptr;
        }
        bool operator!=(const iterator& it) const {
            return ptr != it.ptr;
        }
        const node& operator*() const {
            return *ptr;
        }
        node* operator->() {
            return ptr;
        }
        const Tkey& GetKey() const {
            return ptr->key;
        }
        const Tvalue& GetValue() const {
            return ptr->value;
        }
    };

    iterator begin() const {
        if (!root || root->fict) return iterator(nullptr);

        node* cur = root;
        while (cur->left && !cur->left->fict) {
            cur = cur->left;
        }
        return iterator(cur);
    }

    iterator end() const {
        return iterator(nullptr);
    }

    RBTree() {
        root = new node();
        sz = 0;
    }

    ~RBTree() {
        Destr(root);
    }

    RBTree(const RBTree<Tkey, Tvalue>& t) {
        root = new node();
        root->parent = nullptr;
        sz = t.sz;
        copy(root, t.root);
    }

    RBTree& operator=(const RBTree& t) {
        if (this == &t)
            return *this;

        Destr(root);

        root = new node();
        root->parent = nullptr;
        sz = t.sz;
        copy(root, t.root);
        return *this;
    }

private:
    void rotateLeft(node* x) {
        if (!x || !x->right || x->right->fict) return;

        node* y = x->right;
        x->right = y->left;

        if (y->left && !y->left->fict)
            y->left->parent = x;

        y->parent = x->parent;

        if (!x->parent)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    void rotateRight(node* x) {
        if (!x || !x->left || x->left->fict) return;

        node* y = x->left;
        x->left = y->right;

        if (y->right && !y->right->fict)
            y->right->parent = x;

        y->parent = x->parent;

        if (!x->parent)
            root = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;

        y->right = x;
        x->parent = y;
    }

    void balanceInsert(node* x) {
        if (!x) return;

        // x — корень
        if (x->parent == nullptr) {
            x->color = BLACK;
            return;
        }

        // Родитель чёрный
        if (x->parent->color == BLACK) return;

        // Родитель и дядя красные
        node* u = uncle(x);
        if (u && !u->fict && u->color == RED) {
            x->parent->color = BLACK;
            u->color = BLACK;
            x->parent->parent->color = RED;
            balanceInsert(x->parent->parent); // Проверяем дедушку
            return;
        }

        // Дядя чёрный
        node* g = x->parent->parent;
        if (!g) return;

        if (x->parent == g->left) {
            if (x == x->parent->right) {
                rotateLeft(x->parent);
                x = x->left;
            }
            x->parent->color = BLACK;
            g->color = RED;
            rotateRight(g);
        }
        else {
            if (x == x->parent->left) {
                rotateRight(x->parent);
                x = x->right;
            }
            x->parent->color = BLACK;
            g->color = RED;
            rotateLeft(g);
        }
    }

    node* sibling(node* x) const {
        if (!x || !x->parent || x->parent->fict) return nullptr;
        return (x == x->parent->left) ? x->parent->right : x->parent->left;
    }

    node* uncle(node* x) const {
        if (!x || !x->parent || !x->parent->parent) return nullptr;
        return (x->parent == x->parent->parent->left) ? x->parent->parent->right : x->parent->parent->left;
    }

    void balanceErase(node* x) {
        if (!x || x->fict) return;

        while (x != root && x->color == BLACK) {
            node* s = sibling(x);

            if (s && !s->fict) {
                // Брат красный
                if (s->color == RED) {
                    s->color = BLACK;
                    x->parent->color = RED;
                    if (x == x->parent->left)
                        rotateLeft(x->parent);
                    else
                        rotateRight(x->parent);
                    s = sibling(x);
                }

                // Брат чёрный с чёрными потомками
                if ((!s->left || s->left->fict || s->left->color == BLACK) &&
                    (!s->right || s->right->fict || s->right->color == BLACK)) {
                    s->color = RED;
                    x = x->parent;
                }
                else {
                    // Брат чёрный с красным потомком
                    if (x == x->parent->left &&
                        (!s->right || s->right->fict || s->right->color == BLACK)) {
                        if (s->left && !s->left->fict)
                            s->left->color = BLACK;
                        s->color = RED;
                        rotateRight(s);
                        s = sibling(x);
                    }
                    else if (x == x->parent->right &&
                        (!s->left || s->left->fict || s->left->color == BLACK)) {
                        if (s->right && !s->right->fict)
                            s->right->color = BLACK;
                        s->color = RED;
                        rotateLeft(s);
                        s = sibling(x);
                    }

                    // Перекрашиваем и делаем вращение
                    if (s) {
                        s->color = x->parent->color;
                        x->parent->color = BLACK;
                        if (x == x->parent->left) {
                            if (s->right && !s->right->fict)
                                s->right->color = BLACK;
                            rotateLeft(x->parent);
                        }
                        else {
                            if (s->left && !s->left->fict)
                                s->left->color = BLACK;
                            rotateRight(x->parent);
                        }
                    }
                    x = root;
                }
            }
            else {
                break;
            }
        }
        x->color = BLACK;
    }

    node* prInsert(Tkey key, Tvalue val) {
        node* cur = root;
        node* parent = nullptr;

        // Поиск места для вставки
        while (cur && !cur->fict) {
            parent = cur;
            if (key < cur->key)
                cur = cur->left;
            else if (key > cur->key)
                cur = cur->right;
            else
                return cur; // Ключ уже существует
        }

        // Вставка в фиктивный узел или создание нового
        if (cur && cur->fict) {
            cur->key = key;
            cur->value = val;
            cur->fict = false;
            cur->color = (parent == nullptr) ? BLACK : RED; // Корень всегда чёрный
            createFict(cur);
            sz++;

            if (parent) {
                if (key < parent->key)
                    parent->left = cur;
                else
                    parent->right = cur;
                cur->parent = parent;
            }

            balanceInsert(cur);
            return cur;
        }

        // Создание нового узла
        cur = new node(key, val, RED, false, parent);
        createFict(cur);
        sz++;

        // Привязка к родителю
        if (!parent) {
            root = cur;
            root->color = BLACK;
        }
        else if (key < parent->key) {
            parent->left = cur;
        }
        else {
            parent->right = cur;
        }

        balanceInsert(cur); // Балансировка
        return cur;
    }

    void prErase(Tkey k) {
        node* toDelete = root;
        while (toDelete && !toDelete->fict && toDelete->key != k) {
            toDelete = (k < toDelete->key) ? toDelete->left : toDelete->right;
        }

        if (!toDelete || toDelete->fict) {
            throw std::runtime_error("Element not found");
        }

        if (!toDelete->left->fict && !toDelete->right->fict) {
            node* successor = toDelete->right;
            while (successor->left && !successor->left->fict) {
                successor = successor->left;
            }

            toDelete->key = successor->key;
            toDelete->value = successor->value;
            toDelete = successor;
        }

        node* child = toDelete->left->fict ? toDelete->right : toDelete->left;

        if (sz == 1) {
            delete root->left;
            delete root->right;
            root->key = Tkey();
            root->value = Tvalue();
            root->fict = true;
            root->color = BLACK;
            root->left = root->right = nullptr;
            sz = 0;
            delete toDelete;
            return;
        }

        if (!toDelete->parent) {
            root = child; // Удаляем корень
        }
        else if (toDelete == toDelete->parent->left) {
            toDelete->parent->left = child;
        }
        else {
            toDelete->parent->right = child;
        }

        if (child && !child->fict) {
            child->parent = toDelete->parent;
        }

        if (toDelete->color == BLACK) {
            if (child && !child->fict) {
                child->color = BLACK;
            }
            else {
                balanceErase(toDelete);
            }
        }

        if (toDelete->left && toDelete->left->fict) delete toDelete->left;
        if (toDelete->right && toDelete->right->fict) delete toDelete->right;
        delete toDelete;
        sz--;
    }

public:
    iterator find(Tkey k) const {
        node* cur = root;
        if (root->fict) return end();

        while (cur && !cur->fict) {
            if (cur->key == k) return iterator(cur);
            cur = (k < cur->key) ? cur->left : cur->right;
        }
        return end();
    }

    void clear() {
        delete root->left;
        delete root->right;
        root->left = root->right = nullptr;
        root->fict = true;
        root->color = BLACK;
        sz = 0;
    }

    void insert(Tkey k, Tvalue val) {
        if (find(k) != end())
            throw std::runtime_error("This value already exists");
        prInsert(k, val);
    }

    void erase(Tkey key) {
        prErase(key);
    }

    size_t size() const {
        return sz;
    }

    bool empty() const {
        return sz == 0;
    }
};