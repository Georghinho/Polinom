#include <iostream>
#include <stdexcept>
#include <deque>


template<typename T>
class List {
    struct Node {
        T data;
        Node* next;

        Node(T val) : data(val), next(nullptr) {}
    };

private:
    Node* head;
    Node* tail;
    size_t list_size;

    Node* get_node(size_t index) const {
        if (index >= list_size) {
            throw std::out_of_range("Index out of range");
        }
        Node* current = head;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
        return current;
    }

    void clear() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        head = tail = nullptr;
        list_size = 0;
    }

public:
    List() : head(nullptr), tail(nullptr), list_size(0) {}

    List(int n, T val = T()) : head(nullptr), tail(nullptr), list_size(0) {
        if (n < 0) {
            throw std::invalid_argument("List size cannot be negative");
        }
        for (int i = 0; i < n; ++i) {
            append(val);
        }
    }

    ~List() {
        clear();
    }

    List(const List& other) : head(nullptr), tail(nullptr), list_size(0) {
        Node* current = other.head;
        while (current != nullptr) {
            append(current->data);
            current = current->next;
        }
    }

    List& operator=(const List& other) {
        if (this == &other) {
            return *this;
        }

        clear();

        Node* current = other.head;
        while (current != nullptr) {
            append(current->data);
            current = current->next;
        }

        return *this;
    }

    void append(T val) {
        Node* new_node = new Node(val);
        if (head == nullptr) {
            head = tail = new_node;
        }
        else {
            tail->next = new_node;
            tail = new_node;
        }
        ++list_size;
    }

    T& operator[](size_t index) {
        return get_node(index)->data;
    }

    const T& operator[](size_t index) const {
        return get_node(index)->data;
    }

    size_t size() const {
        return list_size;
    }

    void insert(T val, size_t index) {
        if (index > list_size) {
            throw std::out_of_range("Index out of range");
        }
        if (index == 0) {
            insert_front(val);
            return;
        }
        if (index == list_size) {
            append(val);
            return;
        }
        Node* prev = get_node(index - 1);
        Node* new_node = new Node(val);
        new_node->next = prev->next;
        prev->next = new_node;
        ++list_size;
    }

    void insert_front(T val) {
        Node* new_node = new Node(val);
        if (head == nullptr) {
            head = tail = new_node;
        }
        else {
            new_node->next = head;
            head = new_node;
        }
        ++list_size;
    }

    void erase(size_t index) {
        if (index >= list_size) {
            throw std::out_of_range("Index out of range");
        }
        if (index == 0) {
            erase_front();
            return;
        }
        Node* prev = get_node(index - 1);
        Node* to_delete = prev->next;
        prev->next = to_delete->next;
        if (to_delete == tail) {
            tail = prev;
        }
        delete to_delete;
        --list_size;

        if (list_size == 0) {
            head = tail = nullptr;
        }
    }

    void erase_front() {
        if (head == nullptr) {
            throw std::underflow_error("List is empty");
        }
        Node* to_delete = head;
        head = head->next;
        if (head == nullptr) {
            tail = nullptr;
        }
        delete to_delete;
        --list_size;
    }

    size_t find(T val) const {
        Node* current = head;
        size_t index = 0;
        while (current != nullptr) {
            if (current->data == val) {
                return index;
            }
            current = current->next;
            ++index;
        }
        throw std::logic_error("Value not found in the list");
    }

    T get_first() const {
        if (head == nullptr) {
            throw std::underflow_error("List is empty");
        }
        return head->data;
    }

    bool operator==(const List& other) const {
        if (list_size != other.list_size) {
            return false;
        }
        Node* current1 = head;
        Node* current2 = other.head;
        while (current1 != nullptr) {
            if (current1->data != current2->data) {
                return false;
            }
            current1 = current1->next;
            current2 = current2->next;
        }
        return true;
    }

    friend std::ostream& operator<<(std::ostream& os, const List& list) {
        Node* current = list.head;
        while (current != nullptr) {
            os << current->data << " ";
            current = current->next;
        }
        return os;
    }
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!! Zadacha 3!!!!!!!!!!!!!!!
    //! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    T find_kth_from_end(size_t k) const {
        if (k >= list_size) {
            throw std::out_of_range("Index out of range");
        }
        Node* first = head;
        Node* second = head;
        for (size_t i = 0; i < k; ++i) {
            if (second == nullptr) {
                throw std::out_of_range("Index out of range");
            }
            second = second->next;
        }
        while (second->next != nullptr) {
            first = first->next;
            second = second->next;
        }
        return first->data;
    }

    class Iterator {
        Node* curr;

    public:
        Iterator(Node* node) : curr(node) {}

        Iterator& operator++() {
            if (!curr->next) {
                throw std::out_of_range("Out of range");
            }
            curr = curr->next;
            return *this;
        }

        Iterator operator++(int) {
            if (!curr->next) {
                throw std::out_of_range("Out of range");
            }
            Iterator copy = *this;
            curr = curr->next;
            return copy;
        }

        T& operator*() {
            return curr->data;
        }

        T* operator->() {
            return &(curr->data);
        }

        friend bool operator!=(const Iterator& it1, const Iterator& it2) {
            return it1.curr != it2.curr;
        }

        friend bool operator==(const Iterator& it1, const Iterator& it2) {
            return it1.curr == it2.curr;
        }
    };

    Iterator begin() {
        return Iterator(head);
    }

    Iterator end() {
        return Iterator(nullptr);
    }
};



template<typename T>
class Queue {
private:
    std::deque<T> data;

public:
    Queue(int n = 0, T value = T()) {
        if (n < 0) {
            throw std::invalid_argument("negative size");
        }
        for (int i = 0; i < n; ++i) {
            data.push_back(value);
        }
    }

    void enqueue(const T& val) noexcept {
        data.push_back(val);
    }

    void dequeue() {
        if (isEmpty()) {
            throw std::underflow_error("Queue Empty");
        }
        data.pop_front();
    }

    T& front() {
        if (isEmpty()) {
            throw std::underflow_error("Queue Empty");
        }
        return data.front();
    }

    bool isEmpty() const noexcept {
        return data.empty();
    }

    size_t getSize() const noexcept {
        return data.size();
    }
};

