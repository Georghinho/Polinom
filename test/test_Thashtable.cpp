#include "Thashtable.h"
#include "polinom.h"
#include <gtest.h>

TEST(HashTable, CanInsertAndFindElement) {
    HashO<Polinom, int> table;
    Polinom key("2x^6y^7z^8");
    ASSERT_NO_THROW(table.insert(key, 42));
    EXPECT_EQ(table.find(key), 42);
}

TEST(HashTable, ThrowsWhenInsertingDuplicateKey) {
    HashO<Polinom, int> table;
    Polinom key("2x^6y^7z^8");
    table.insert(key, 42);
    EXPECT_ANY_THROW(table.insert(key, 43));
}

TEST(HashTable, CanEraseElement) {
    HashO<Polinom, int> table;
    Polinom key("2x^6y^7z^8");
    table.insert(key, 42);
    ASSERT_NO_THROW(table.erase(key));
    EXPECT_ANY_THROW(table.find(key));
}

TEST(HashTable, ThrowsWhenErasingNonExistentKey) {
    HashO<Polinom, int> table;
    Polinom key("2x^6y^7z^8");
    EXPECT_ANY_THROW(table.erase(key));
}

TEST(HashTable, CanAccessElementViaOperator) {
    HashO<Polinom, int> table;
    Polinom key("2x^6y^7z^8");
    table.insert(key, 42);
    EXPECT_EQ(table[key], 42);
}

TEST(HashTable, CanModifyElementViaOperator) {
    HashO<Polinom, int> table;
    Polinom key("2x^6y^7z^8");
    table.insert(key, 42);
    table[key] = 43;
    EXPECT_EQ(table.find(key), 43);
}

TEST(HashTable, CanHandleCollisions) {
    HashO<Polinom, int> table(5);

    Polinom key1("x");
    Polinom key2("y");
    Polinom key3("x+y");

    ASSERT_NO_THROW(table.insert(key1, 1));
    ASSERT_NO_THROW(table.insert(key2, 2));
    ASSERT_NO_THROW(table.insert(key3, 3));

    EXPECT_EQ(table.getSize(), 3);

    EXPECT_EQ(table.find(key1), 1);
    EXPECT_EQ(table.find(key2), 2);
    EXPECT_EQ(table.find(key3), 3);
}

TEST(HashTable, CanRehashWhenFull) {
    HashO<Polinom, int> table(4);

    Polinom key1("x");
    Polinom key2("y");
    Polinom key3("z");
    Polinom key4("x+y");
    Polinom key5("x+z");

    int initial_capacity = table.getCapacity();

    ASSERT_NO_THROW(table.insert(key1, 1));
    ASSERT_NO_THROW(table.insert(key2, 2));
    ASSERT_NO_THROW(table.insert(key3, 3));
    ASSERT_NO_THROW(table.insert(key4, 4));
    ASSERT_NO_THROW(table.insert(key5, 5));

    EXPECT_EQ(table.find(key1), 1);
    EXPECT_EQ(table.find(key2), 2);
    EXPECT_EQ(table.find(key3), 3);
    EXPECT_EQ(table.find(key4), 4);
    EXPECT_EQ(table.find(key5), 5);

    EXPECT_GT(table.getCapacity(), initial_capacity);

    EXPECT_EQ(table.getSize(), 5);
}

TEST(HashTable, IteratorWorksCorrectly) {
    HashO<Polinom, int> table;
    Polinom key1("x");
    Polinom key2("y");
    table.insert(key1, 1);
    table.insert(key2, 2);

    int sum = 0;
    for (auto it = table.begin(); it != table.end(); ++it) {
        sum += (*it).second;  // Используем оператор * вместо ->
    }
    EXPECT_EQ(sum, 3); // 1 + 2
}

TEST(HashTable, CanStoreDifferentValueTypes) {
    HashO<Polinom, std::string> table;
    Polinom key("2x^6y^7z^8");
    table.insert(key, "test");
    EXPECT_EQ(table.find(key), "test");
}

TEST(HashTable, HandlesPolinomWithDifferentForms) {
    HashO<Polinom, int> table;
    Polinom key1("x+y+z");
    Polinom key2("y+z+x");
    Polinom key3("z+x+y");

    table.insert(key1, 1);
    EXPECT_EQ(table.find(key2), 1); // Same polynom, different order
    EXPECT_EQ(table.find(key3), 1); // Same polynom, different order
}

TEST(HashTable, HandlesZeroPolinom) {
    HashO<Polinom, int> table;
    Polinom key("0");
    table.insert(key, 42);
    EXPECT_EQ(table.find(key), 42);
}
