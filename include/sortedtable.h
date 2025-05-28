#pragma once

#include "polinom.h"
#include <sstream>
#include <algorithm>
#include <utility>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <iterator>

class SortedTable {
private:
    std::vector<std::pair<std::string, Polinom>> data;

public:
    SortedTable() = default;

    SortedTable(const std::string& id, const Polinom& pol) {
        insert(id, pol);
    }

    bool empty() const {
        return data.empty();
    }

    int size() const {
        return static_cast<int>(data.size());
    }

    int count() const {
        return size();
    }

    void print() const {
        for (const auto& elem : data) {
            std::cout << elem.first << " " << elem.second << std::endl;
        }
    }

    Polinom find(const std::string& id) const {
        auto pos = std::lower_bound(data.begin(), data.end(), id,
            [](const auto& elem, const auto& key) {
                return elem.first < key;
            });
        if (pos == data.end() || pos->first != id) {
            throw std::runtime_error("Key '" + id + "' not found in table");
        }
        return pos->second;
    }

    void insert(const std::string& id, const Polinom& pol) {
        auto pos = std::lower_bound(data.begin(), data.end(), id,
            [](const auto& elem, const auto& key) {
                return elem.first < key;
            });
        if (pos != data.end() && pos->first == id) {
            throw std::runtime_error("Duplicate key '" + id + "' detected");
        }
        data.insert(pos, { id, pol });
    }

    void erase(const std::string& id) {
        auto pos = std::lower_bound(data.begin(), data.end(), id,
            [](const auto& elem, const auto& key) {
                return elem.first < key;
            });
        if (pos == data.end() || pos->first != id) {
            throw std::runtime_error("Cannot erase - key '" + id + "' not found");
        }
        data.erase(pos);
    }

    bool operator==(const SortedTable& other) const {
        return data == other.data;
    }

    bool operator!=(const SortedTable& other) const {
        return !(*this == other);
    }

    class Iterator {
    private:
        std::vector<std::pair<std::string, Polinom>>::iterator iter;
    public:
        Iterator(std::vector<std::pair<std::string, Polinom>>::iterator it) : iter(it) {}
        Iterator() = default;

        auto& operator*() { return *iter; }
        auto* operator->() { return &(*iter); }
        Iterator& operator++() { ++iter; return *this; }
        bool operator!=(const Iterator& other) const { return iter != other.iter; }
        bool operator==(const Iterator& other) const { return iter == other.iter; }
    };

    Iterator begin() { return Iterator(data.begin()); }
    Iterator end() { return Iterator(data.end()); }
};