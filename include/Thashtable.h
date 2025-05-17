#pragma once
#include <vector>
#include <cmath>
#include <ctime>
#include <stdexcept>
#include "polinom.h"

template <class Tkey, class Tvalue>
class HashO {
private:
    struct tuple {
        Tkey key;
        Tvalue value;
        bool flag;      // Флаг удаления (true - удалён)
        bool empty;     // Флаг пустой ячейки

        tuple() : flag(false), empty(true) {}
        tuple(Tkey k) : key(k), value(Tvalue()), flag(false), empty(true) {}
        tuple(Tkey k, Tvalue v) : key(k), value(v), flag(false), empty(false) {}

        Tkey getKey() const { return key; }
        Tvalue getValue() const { return value; }
        bool getFlag() const { return flag; }
        bool getEmpty() const { return empty; }
    };

    std::vector<tuple> table;
    int size;
    int capacity;
    long long a, b;
    const int simpleNum = 1'000'000'007;
    const double LOAD_FACTOR = 0.6; // Порог для рехеширования

    void rehash() {
        int newCapacity = capacity * 2 + 1;
        std::vector<tuple> newTable(newCapacity);

        auto oldTable = std::move(table);
        size = 0; // Сбрасываем счётчик
        capacity = newCapacity;
        table = std::vector<tuple>(newCapacity);

        // Перенос элементов в новую таблицу
        for (const auto& item : oldTable) {
            if (!item.getEmpty() && !item.getFlag()) {
                insert(item.getKey(), item.getValue());
            }
        }
    }

public:
    HashO(int initialCapacity = 157) : size(0), capacity(initialCapacity) {
        if (capacity <= 0) capacity = 157;
        table.resize(capacity);
        std::srand(std::time(nullptr));
        a = (std::abs(std::rand()) % (simpleNum - 1)) + 1;
        b = std::abs(std::rand()) % simpleNum;
    }

    int Hash(const Polinom& p) const {
        unsigned long long hash = 0;
        for (const auto& m : p.getMonoms()) {
            hash = (hash * 31 + m.degree) % simpleNum;
            long long coeff_hash = static_cast<long long>(std::round(m.coeff * 1000));
            hash = (hash * 31 + coeff_hash) % simpleNum;
        }
        return ((a * hash + b) % simpleNum) % capacity;
    }

    int Hash(const int& key) const {
        return ((a * key + b) % simpleNum) % capacity;
    }

    void insert(const Tkey& key, const Tvalue& value) {
        if (size >= capacity * LOAD_FACTOR) {
            rehash();
        }

        int index = Hash(key);
        int startIndex = index;
        bool foundDeleted = false;
        int deletedIndex = -1;

        while (!table[index].getEmpty()) {
            if (!table[index].getFlag() && table[index].getKey() == key) {
                throw std::runtime_error("Key already exists");
            }
            if (table[index].getFlag() && !foundDeleted) {
                foundDeleted = true;
                deletedIndex = index;
            }
            index = (index + 1) % capacity;
            if (index == startIndex) {
                throw std::runtime_error("Hash table is full");
            }
        }

        if (foundDeleted) {
            index = deletedIndex;
        }

        table[index] = tuple(key, value);
        size++;
    }

    Tvalue find(const Tkey& key) const {
        int index = Hash(key);
        int startIndex = index;

        do {
            if (table[index].getEmpty() && !table[index].getFlag()) {
                throw std::runtime_error("Key not found");
            }
            if (!table[index].getFlag() && !table[index].getEmpty() && table[index].getKey() == key) {
                return table[index].getValue();
            }
            index = (index + 1) % capacity;
        } while (index != startIndex);

        throw std::runtime_error("Key not found");
    }

    void erase(const Tkey& key) {
        int index = Hash(key);
        int startIndex = index;

        do {
            if (table[index].getEmpty() && !table[index].getFlag()) {
                throw std::runtime_error("Key not found");
            }
            if (!table[index].getFlag() && !table[index].getEmpty() && table[index].getKey() == key) {
                table[index].flag = true;
                size--;
                return;
            }
            index = (index + 1) % capacity;
        } while (index != startIndex);

        throw std::runtime_error("Key not found");
    }

    Tvalue& operator[](const Tkey& key) {
        int index = Hash(key);
        int startIndex = index;

        do {
            if (table[index].getEmpty() && !table[index].getFlag()) {
                throw std::runtime_error("Key not found");
            }
            if (!table[index].getFlag() && !table[index].getEmpty() && table[index].getKey() == key) {
                return table[index].value;
            }
            index = (index + 1) % capacity;
        } while (index != startIndex);

        throw std::runtime_error("Key not found");
    }

    int getSize() const { return size; }
    int getCapacity() const { return capacity; }

    class iterator {
        typename std::vector<tuple>::iterator current;
        typename std::vector<tuple>::iterator end;

    public:
        iterator(typename std::vector<tuple>::iterator it,
            typename std::vector<tuple>::iterator e)
            : current(it), end(e) {
            while (current != end && (current->getEmpty() || current->getFlag())) {
                ++current;
            }
        }

        iterator& operator++() {
            do {
                ++current;
            } while (current != end && (current->getEmpty() || current->getFlag()));
            return *this;
        }

        std::pair<const Tkey&, Tvalue&> operator*() {
            return { current->key, current->value };
        }

        bool operator==(const iterator& other) const {
            return current == other.current;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };

    iterator begin() {
        return iterator(table.begin(), table.end());
    }

    iterator end() {
        return iterator(table.end(), table.end());
    }
};
