//#include <gtest.h>
//#include "three.h"
//#include <vector>
//#include <stdexcept>
//#include <string>
//
//#define MAX_NODES 500000
//
//TEST(AVLTree, Initialization) {
//    ASSERT_NO_THROW(AVLTree());
//}
//
//TEST(AVLTree, Insertion) {
//    AVLTree tree;
//    tree.insert("5", Polinom("5"));
//    tree.insert("8", Polinom("8"));
//    EXPECT_EQ(tree.size(), 2);
//}
//
//TEST(AVLTree, InsertAndFind) {
//    AVLTree tree;
//    tree.insert("15", Polinom("Fifteen"));
//    tree.insert("25", Polinom("TwentyFive"));
//    tree.insert("7", Polinom("Seven"));
//
//    EXPECT_EQ(tree.find("15"), Polinom("Fifteen"));
//    EXPECT_EQ(tree.find("7"), Polinom("Seven"));
//    EXPECT_EQ(tree.find("25"), Polinom("TwentyFive"));
//}
//
//TEST(AVLTree, InsertDuplicateKey) {
//    AVLTree tree;
//    tree.insert("12", Polinom("Twelve"));
//    EXPECT_THROW(tree.insert("12", Polinom("Duplicate")), std::runtime_error);
//}
//
//TEST(AVLTree, Erase) {
//    AVLTree tree;
//    tree.insert("12", Polinom("Twelve"));
//    tree.insert("24", Polinom("TwentyFour"));
//    tree.insert("6", Polinom("Six"));
//
//    tree.erase("6");
//    EXPECT_EQ(tree.size(), 2);
//}
//
//TEST(AVLTree, CompareTrees) {
//    AVLTree tree1, tree2;
//    tree1.insert("15", Polinom("Fifteen"));
//    tree1.insert("30", Polinom("Thirty"));
//    tree2.insert("15", Polinom("Fifteen"));
//    tree2.insert("30", Polinom("Thirty"));
//    EXPECT_TRUE(tree1 == tree2);
//}
//
//TEST(AVLTree, Size) {
//    AVLTree tree;
//    EXPECT_EQ(tree.size(), 0);
//    tree.insert("18", Polinom("Eighteen"));
//    EXPECT_EQ(tree.size(), 1);
//    tree.insert("36", Polinom("ThirtySix"));
//    EXPECT_EQ(tree.size(), 2);
//    tree.erase("36");
//    EXPECT_EQ(tree.size(), 1);
//}
//
//TEST(Iterator, TreeIsEmpty) {
//    AVLTree tree;
//    EXPECT_EQ(tree.begin(), tree.end());
//}
//
//TEST(Iterator, OnlyOneElement) {
//    AVLTree tree;
//    tree.insert("99", Polinom("Answer"));
//    auto it = tree.begin();
//    EXPECT_EQ(it.key(), "99");
//    EXPECT_EQ(it.value(), Polinom("Answer"));
//    ++it;
//    EXPECT_EQ(it, tree.end());
//}
//
//
//TEST(Iterator, CheckEmptyEnd) {
//    AVLTree tree;
//    auto it = tree.end();
//    EXPECT_THROW({ auto k = it.key(); }, std::out_of_range);
//    EXPECT_THROW({ auto v = it.value(); }, std::out_of_range);
//}
//
