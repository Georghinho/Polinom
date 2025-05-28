#pragma once
#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <functional>
#include <stdexcept>
#include <algorithm>
#include "polinom.h"

class PolinomHashTable {
private:
    std::vector<std::vector<std::tuple<std::string, Polinom, bool>>> table;
    size_t count_element = 0;
    double MAX_LOAD_FACTOR = 0.7;

    bool fullTable() {
        if (table.empty()) return false;
        double loadFactor = static_cast<double>(count_element) / table.size();
        return (loadFactor > MAX_LOAD_FACTOR);
    }

    void rehash() {
        size_t new_size = table.size() * 2 + 1;
        std::vector<std::vector<std::tuple<std::string, Polinom, bool>>> new_table(new_size);
        for (auto& bucket : table) {
            for (auto& entry : bucket) {
                if (std::get<2>(entry)) {
                    size_t new_index = std::hash<std::string>{}(std::get<0>(entry)) % new_size;
                    new_table[new_index].push_back(entry);
                }
            }
        }
        table = std::move(new_table);
    }

public:
    PolinomHashTable() : count_element(0), MAX_LOAD_FACTOR(0.7) {
        table.resize(1);
    }

    PolinomHashTable(int n) : count_element(0), MAX_LOAD_FACTOR(0.7) {
        if (n <= 0) {
            n = 1;
        }
        table.resize(n);
    }

    size_t hash_function(const std::string& key) const {
        return std::hash<std::string>{}(key) % table.size();
    }

    void insert(const std::string& key, const Polinom& value) {
        if (fullTable()) {
            rehash();
        }

        size_t index = hash_function(key);
        for (auto& entry : table[index]) {
            if (std::get<0>(entry) == key) {
                if (!std::get<2>(entry)) {
                    std::get<1>(entry) = value;
                    std::get<2>(entry) = true;
                    count_element++;
                    return;
                }
                else {
                    std::get<1>(entry) = value;
                    return;
                }
            }
        }
        table[index].push_back(std::make_tuple(key, value, true));
        count_element++;
    }

    Polinom find(const std::string& key) {
        if (table.empty()) {
            throw std::runtime_error("Element not found: empty table");
        }
        size_t index = hash_function(key);
        for (auto& entry : table[index]) {
            if (std::get<0>(entry) == key && std::get<2>(entry)) {
                return std::get<1>(entry);
            }
        }
        throw std::runtime_error("Element '" + key + "' not found");
    }

    void erase(const std::string& key) {
        if (table.empty()) {
            throw std::runtime_error("Cannot erase: table is empty");
        }
        size_t index = hash_function(key);
        for (auto& entry : table[index]) {
            if (std::get<0>(entry) == key && std::get<2>(entry)) {
                std::get<2>(entry) = false;
                count_element--;
                return;
            }
        }
        throw std::runtime_error("Cannot erase: element '" + key + "' not found");
    }

    int size() {
        return table.size();
    }

    int count() {
        return count_element;
    }

    bool empty() {
        return count_element == 0;
    }

    // Новый метод для печати всех сохранённых полиномов
    void print() const {
        for (const auto& bucket : table) {
            for (const auto& entry : bucket) {
                if (std::get<2>(entry)) {
                    std::cout << std::get<0>(entry) << ": " << std::get<1>(entry) << "\n";
                }
            }
        }
    }
};
