//#include <gtest.h>
//#include "hashtable.h"
//#include <stdexcept>
//#include <string>
//#include <vector>
//#include <iostream>
//
//TEST(HashTable, can_create_empty_hash_table) {
//    PolinomHashTable t1;
//    ASSERT_TRUE(t1.empty());
//}
//
//TEST(HashTable, can_create_hash_table_with_size) {
//    PolinomHashTable t1(10);
//    EXPECT_EQ(t1.size(), 10);
//    EXPECT_TRUE(t1.empty());
//}
//
//TEST(HashTable, can_insert_element) {
//    PolinomHashTable t1;
//    t1.insert("1", Polinom("1"));
//    ASSERT_EQ(t1.count(), 1);
//}
//
//TEST(HashTable, can_delete_element_from_table) {
//    PolinomHashTable t1;
//    t1.insert("1", Polinom("1"));
//    t1.erase("1");
//    ASSERT_TRUE(t1.empty());
//}
//
//TEST(HashTable, cant_delete_element_from_empty_table) {
//    PolinomHashTable t1;
//    EXPECT_ANY_THROW(t1.erase("15"));
//}
//
//TEST(HashTable, EraseKey) {
//    PolinomHashTable table;
//    table.insert("key1", Polinom("value1"));
//    table.erase("key1");
//    EXPECT_THROW(table.find("key1"), std::runtime_error);
//    EXPECT_EQ(table.count(), 0);
//}
//
//TEST(HashTable, CountAndSize) {
//    PolinomHashTable table(5);
//    EXPECT_EQ(table.size(), 5);
//
//    table.insert("key1", Polinom("value1"));
//    table.insert("key2", Polinom("value2"));
//    table.insert("key3", Polinom("value3"));
//    EXPECT_EQ(table.count(), 3);
//
//    table.erase("key2");
//    EXPECT_EQ(table.count(), 2);
//}
//
//TEST(HashTable, can_insert_and_delete_element_2_) {
//    PolinomHashTable t1;
//    t1.insert("1", Polinom("1"));
//    t1.insert("2", Polinom("4"));
//    t1.erase("2");
//    ASSERT_EQ(t1.count(), 1);
//}
//
//TEST(HashTable, can_insert_and_delete_element_3) {
//    PolinomHashTable t1;
//    t1.insert("1", Polinom("1"));
//    t1.insert("2", Polinom("4"));
//    t1.insert("3", Polinom("4"));
//    t1.erase("3");
//    ASSERT_EQ(t1.count(), 2);
//}
//
//TEST(HashTable, can_insert_and_delete_element_2) {
//    PolinomHashTable t1;
//    t1.insert("1", Polinom("1"));
//    t1.insert("2", Polinom("4"));
//    t1.insert("3", Polinom("3"));
//    t1.erase("2");
//    ASSERT_EQ(t1.count(), 2);
//}
//
//TEST(HashTable, can_insert_and_delete_element_1) {
//    PolinomHashTable t1;
//    t1.insert("1", Polinom("1"));
//    std::cout << t1.count() << std::endl;
//    t1.insert("2", Polinom("4"));
//    std::cout << t1.count() << std::endl;
//    t1.insert("3", Polinom("3"));
//    std::cout << t1.count() << std::endl;
//    t1.erase("1");
//    std::cout << t1.count() << std::endl;
//    ASSERT_EQ(t1.count(), 2);
//}
//
//TEST(HashTable, can_find_element) {
//    PolinomHashTable t1;
//    t1.insert("1", Polinom("1"));
//    ASSERT_EQ(t1.find("1"), Polinom("1"));
//}
//
