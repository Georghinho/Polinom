#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <cctype>

struct Monom {
    int degree = 0;
    double coeff = 0.0;

    Monom() = default;
    Monom(int deg, double c) : degree(deg), coeff(c) {
        validateDegree();
    }

    explicit Monom(const std::string& str) {
        if (str.empty() || str == "0") {
            coeff = 0.0;
            degree = 0;
            return;
        }
        parseWithStateMachine(str);
        validateDegree();
    }

private:
    void parseWithStateMachine(const std::string& str) {
        size_t pos = 0;
        while (pos < str.size() && std::isspace(str[pos])) pos++;

        bool negative = false;
        if (pos < str.size() && (str[pos] == '+' || str[pos] == '-')) {
            if (str[pos] == '-') {
                negative = true;
            }
            pos++;
            while (pos < str.size() && std::isspace(str[pos])) pos++;
        }

        double coef = 1.0;
        if (pos < str.size() && (std::isdigit(str[pos]) || str[pos] == '.')) {
            size_t end_pos = pos;
            try {
                coef = std::stod(str.substr(pos), &end_pos);
            }
            catch (...) {
                throw std::runtime_error("Invalid coefficient format");
            }
            pos += end_pos;
            while (pos < str.size() && std::isspace(str[pos])) pos++;
        }
        if (negative) coef = -coef;

        int x_pow = 0, y_pow = 0, z_pow = 0;
        while (pos < str.size()) {
            char var = str[pos];
            if (var == 'x' || var == 'y' || var == 'z') {
                pos++;
                while (pos < str.size() && std::isspace(str[pos])) pos++;
                int exponent = 1;
                if (pos < str.size() && str[pos] == '^') {
                    pos++;
                    while (pos < str.size() && std::isspace(str[pos])) pos++;
                    exponent = parseExponent(str, pos);
                }
                if (var == 'x') x_pow += exponent;
                else if (var == 'y') y_pow += exponent;
                else if (var == 'z') z_pow += exponent;
                while (pos < str.size() && std::isspace(str[pos])) pos++;
            }
            else {
                throw std::runtime_error("Unexpected character in monom: " + std::string(1, var));
            }
        }
        if (x_pow > 9 || y_pow > 9 || z_pow > 9)
            throw std::runtime_error("Degree overflow in monom: exponent too large (max 9)");

        coeff = coef;
        degree = x_pow * 100 + y_pow * 10 + z_pow;
    }

    int parseExponent(const std::string& str, size_t& pos) {
        if (pos >= str.size() || !std::isdigit(str[pos])) {
            throw std::runtime_error("Expected exponent after '^'");
        }
        int exponent = 0;
        while (pos < str.size() && std::isdigit(str[pos])) {
            exponent = exponent * 10 + (str[pos] - '0');
            pos++;
        }
        if (exponent > 9)
            throw std::runtime_error("Exponent too large (max 9)");
        return exponent;
    }

    void validateDegree() const {
        int x = degree / 100;
        int y = (degree / 10) % 10;
        int z = degree % 10;
        if (x > 9 || y > 9 || z > 9)
            throw std::runtime_error("Degree overflow in monom");
    }

public:
    bool operator>(const Monom& other) const { return degree > other.degree; }
    bool operator<(const Monom& other) const { return degree < other.degree; }
    bool operator==(const Monom& other) const {
        return std::abs(coeff - other.coeff) < 1e-10 && degree == other.degree;
    }
    bool operator!=(const Monom& other) const { return !(*this == other); }

    Monom operator+(const Monom& other) const {
        if (degree != other.degree) {
            throw std::runtime_error("Cannot add monoms with different degrees");
        }
        return Monom(degree, coeff + other.coeff);
    }

    Monom operator-(const Monom& other) const {
        if (degree != other.degree) {
            throw std::runtime_error("Cannot subtract monoms with different degrees");
        }
        return Monom(degree, coeff - other.coeff);
    }

    Monom operator*(double val) const {
        if (std::abs(val) < 1e-10)
            return Monom(0, 0.0);
        return Monom(degree, coeff * val);
    }

    Monom operator*(const Monom& other) const {
        int new_degree = degree + other.degree;
        if ((new_degree / 100) > 9 || ((new_degree / 10) % 10 > 9 || (new_degree % 10) > 9))
            throw std::runtime_error("Degree overflow in monom multiplication");
        return Monom(new_degree, coeff * other.coeff);
    }

    friend std::ostream& operator<<(std::ostream& os, const Monom& m) {
        if (m.coeff == 1.0 && m.degree != 0) {
        }
        else if (m.coeff == -1.0 && m.degree != 0) {
            os << "-";
        }
        else {
            os << m.coeff;
        }
        auto printVar = [&](char var, int power) {
            if (power > 0) {
                os << var;
                if (power > 1) os << "^" << power;
            }
            };
        printVar('x', m.degree / 100);
        printVar('y', (m.degree / 10) % 10);
        printVar('z', m.degree % 10);
        return os;
    }
};

class Polinom {
private:
    std::vector<Monom> monoms;

    void parsePolinom(const std::string& str) {
        std::vector<std::string> terms;
        std::string current;
        bool has_content = false;
        for (char c : str) {
            if (c == '+' || c == '-') {
                if (has_content) {
                    terms.push_back(current);
                    current.clear();
                }
                if (c == '-') current.push_back('-');
                has_content = false;
            }
            else if (!std::isspace(c)) {
                current.push_back(c);
                has_content = true;
            }
        }
        if (has_content) terms.push_back(current);

        for (const auto& term : terms) {
            if (!term.empty()) {
                Monom m(term);
                if (std::fabs(m.coeff) > 1e-10)
                    monoms.push_back(m);
            }
        }
        combineLikeTerms();
    }

    void combineLikeTerms() {
        if (monoms.empty()) return;
        std::sort(monoms.begin(), monoms.end(), std::greater<Monom>());
        std::vector<Monom> combined;
        Monom current = monoms[0];
        for (size_t i = 1; i < monoms.size(); ++i) {
            if (monoms[i].degree == current.degree) {
                current.coeff += monoms[i].coeff;
            }
            else {
                if (std::fabs(current.coeff) > 1e-10)
                    combined.push_back(current);
                current = monoms[i];
            }
        }
        if (std::fabs(current.coeff) > 1e-10)
            combined.push_back(current);
        monoms = std::move(combined);
    }

public:
    Polinom() = default;
    explicit Polinom(const std::string& str) {
        if (!str.empty())
            parsePolinom(str);
    }

    Polinom(const Polinom&) = default;
    Polinom& operator=(const Polinom&) = default;

    Polinom operator+(const Polinom& other) const {
        Polinom result;
        size_t i = 0, j = 0;
        while (i < monoms.size() && j < other.monoms.size()) {
            if (monoms[i].degree == other.monoms[j].degree) {
                Monom sum = monoms[i] + other.monoms[j];
                if (std::fabs(sum.coeff) > 1e-10)
                    result.monoms.push_back(sum);
                ++i; ++j;
            }
            else if (monoms[i].degree > other.monoms[j].degree) {
                result.monoms.push_back(monoms[i]);
                ++i;
            }
            else {
                result.monoms.push_back(other.monoms[j]);
                ++j;
            }
        }
        while (i < monoms.size()) {
            result.monoms.push_back(monoms[i]);
            ++i;
        }
        while (j < other.monoms.size()) {
            result.monoms.push_back(other.monoms[j]);
            ++j;
        }
        return result;
    }

    Polinom operator-(const Polinom& other) const {
        Polinom result;
        size_t i = 0, j = 0;
        while (i < monoms.size() && j < other.monoms.size()) {
            if (monoms[i].degree == other.monoms[j].degree) {
                Monom diff = monoms[i] - other.monoms[j];
                if (std::fabs(diff.coeff) > 1e-10)
                    result.monoms.push_back(diff);
                ++i; ++j;
            }
            else if (monoms[i].degree > other.monoms[j].degree) {
                result.monoms.push_back(monoms[i]);
                ++i;
            }
            else {
                result.monoms.push_back(Monom(other.monoms[j].degree, -other.monoms[j].coeff));
                ++j;
            }
        }
        while (i < monoms.size()) {
            result.monoms.push_back(monoms[i]);
            ++i;
        }
        while (j < other.monoms.size()) {
            result.monoms.push_back(Monom(other.monoms[j].degree, -other.monoms[j].coeff));
            ++j;
        }
        return result;
    }

    Polinom operator*(const Polinom& other) const {
        Polinom result;
        for (const auto& m1 : monoms) {
            Polinom temp;
            for (const auto& m2 : other.monoms) {
                try {
                    Monom product = m1 * m2;
                    if (std::fabs(product.coeff) > 1e-10)
                        temp.monoms.push_back(product);
                }
                catch (const std::runtime_error& e) {
                    throw std::runtime_error(std::string("Multiplication error: ") + e.what());
                }
            }
            result = result + temp;
        }
        return result;
    }

    Polinom operator*(double scalar) const {
        Polinom result;
        for (const auto& m : monoms) {
            Monom product = m * scalar;
            if (std::fabs(product.coeff) > 1e-10)
                result.monoms.push_back(product);
        }
        return result;
    }

    size_t size() const { return monoms.size(); }
    bool empty() const { return monoms.empty(); }
    const std::vector<Monom>& getMonoms() const { return monoms; }

    bool operator==(const Polinom& other) const {
        if (monoms.size() != other.monoms.size())
            return false;
        for (size_t i = 0; i < monoms.size(); ++i)
            if (monoms[i] != other.monoms[i])
                return false;
        return true;
    }
    bool operator!=(const Polinom& other) const { return !(*this == other); }

    friend std::ostream& operator<<(std::ostream& os, const Polinom& p) {
        if (p.monoms.empty()) {
            os << "0";
            return os;
        }
        for (size_t i = 0; i < p.monoms.size(); ++i) {
            if (i > 0 && p.monoms[i].coeff > 0)
                os << "+";
            os << p.monoms[i];
        }
        return os;
    }

    void print() const {
        std::cout << *this << std::endl;
    }
};
