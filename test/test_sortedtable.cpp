#include <gtest.h>
#include "sortedtable.h"
#include <stdexcept>
#include <string>
#include <vector>

TEST(SortedTable, CanCreateEmptyTable) {
    SortedTable t;
    ASSERT_TRUE(t.empty());
}

TEST(SortedTable, CanInsertElement) {
    SortedTable t;
    t.insert("key5", Polinom("5"));
    ASSERT_EQ(t.count(), 1);
}

TEST(SortedTable, EraseKey) {
    SortedTable t;
    t.insert("key7", Polinom("value7"));
    t.erase("key7");
    EXPECT_THROW(t.find("key7"), std::runtime_error);
    EXPECT_EQ(t.count(), 0);
}

TEST(SortedTable, CountAndSize) {
    SortedTable t;
    EXPECT_EQ(t.count(), 0);

    t.insert("key10", Polinom("value10"));
    t.insert("key20", Polinom("value20"));
    t.insert("key30", Polinom("value30"));
    EXPECT_EQ(t.count(), 3);

    t.erase("key20");
    EXPECT_EQ(t.count(), 2);
    EXPECT_EQ(t.size(), t.count());
}

TEST(SortedTable, CanInsertAndDeleteMultipleElements) {
    SortedTable t;
    t.insert("key100", Polinom("100"));
    t.insert("key200", Polinom("400"));
    t.erase("key200");
    ASSERT_EQ(t.count(), 1);

    t.insert("key300", Polinom("300"));
    t.insert("key400", Polinom("500"));
    t.erase("key300");
    ASSERT_EQ(t.count(), 2);
}

TEST(SortedTable, CanFindElement) {
    SortedTable t;
    t.insert("key42", Polinom("42"));
    ASSERT_EQ(t.find("key42"), Polinom("42"));
}

TEST(SortedTable, DuplicateInsertThrows) {
    SortedTable t;
    t.insert("dupKey", Polinom("10"));
    EXPECT_THROW(t.insert("dupKey", Polinom("20")), std::runtime_error);
}

TEST(SortedTable, IteratorWorks) {
    SortedTable t;
    t.insert("x", Polinom("x"));
    t.insert("z", Polinom("z"));
    t.insert("y", Polinom("y"));

    std::vector<std::string> keys;
    for (auto it = t.begin(); it != t.end(); ++it) {
        keys.push_back(it->first);
    }

    std::vector<std::string> expected = { "x", "y", "z" };
    ASSERT_EQ(keys, expected);
}