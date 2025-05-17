#include "Tthree.h"
#include <gtest.h>

// ����� ������� ���������
TEST(RBTreeTest, InsertAddsNewElementAndIncreasesSize) {
    RBTree<std::string, Polinom> tree;
    Polinom p("x + y + z");

    EXPECT_NO_THROW(tree.insert("poly1", p));
    EXPECT_EQ(tree.size(), 1);
    EXPECT_EQ(tree.find("poly1")->value, p);
}

TEST(RBTreeTest, InsertThrowsForDuplicateKey) {
    RBTree<std::string, Polinom> tree;
    Polinom p1("x^2 + y");
    Polinom p2("y^2 + z");

    tree.insert("poly1", p1);
    EXPECT_THROW(tree.insert("poly1", p2), std::runtime_error);
}

//���� �������� ���������
TEST(RBTreeTest, EraseThrowsForNonexistentKey) {
    RBTree<std::string, Polinom> tree;
    EXPECT_THROW(tree.erase("nonexistent"), std::runtime_error);
}

// ����� ������
TEST(RBTreeTest, FindReturnsCorrectIteratorForExistingElement) {
    RBTree<std::string, Polinom> tree;
    Polinom p("x^3 + y^3 + z^3");
    tree.insert("poly1", p);

    auto it = tree.find("poly1");
    EXPECT_NE(it, tree.end());
    EXPECT_EQ(it->key, "poly1");
    EXPECT_EQ(it->value, p);
}

TEST(RBTreeTest, FindReturnsEndForNonexistentElement) {
    RBTree<std::string, Polinom> tree;
    EXPECT_EQ(tree.find("nonexistent"), tree.end());
}

// ����� ������ � ����������� ����������
TEST(RBTreeTest, HandlesMultipleInsertionsAndDeletions) {
    RBTree<std::string, Polinom> tree;
    Polinom p1("x");
    Polinom p2("y");
    Polinom p3("z");

    tree.insert("p1", p1);
    tree.insert("p2", p2);
    tree.insert("p3", p3);

    EXPECT_EQ(tree.size(), 3);

    tree.erase("p2");
    EXPECT_EQ(tree.size(), 2);
    EXPECT_EQ(tree.find("p1")->value, p1);
    EXPECT_EQ(tree.find("p3")->value, p3);
}

// ����� � ������������ ����������
TEST(RBTreeTest, StoresAndRetrievesComplexPolynomials) {
    RBTree<std::string, Polinom> tree;
    Polinom p("3.5x^5y^2z - 2.1x^3y^4z^2 + 7.8x^2y^3z^4 - 1.2");

    tree.insert("complex", p);
    auto found = tree.find("complex");

    ASSERT_NE(found, tree.end());
    EXPECT_EQ(found->value, p);
}

// ����� �����������
TEST(RBTreeTest, CopyConstructorCreatesEqualTree) {
    RBTree<std::string, Polinom> original;
    Polinom p("x^2 + y^2 + z^2");
    original.insert("p1", p);

    RBTree<std::string, Polinom> copy(original);
    EXPECT_EQ(original.size(), copy.size());
    EXPECT_EQ(original.find("p1")->value, copy.find("p1")->value);
}

TEST(RBTreeTest, AssignmentOperatorCreatesEqualTree) {
    RBTree<std::string, Polinom> original;
    Polinom p("x^3 + y^3 + z^3");
    original.insert("p1", p);

    RBTree<std::string, Polinom> assigned;
    assigned = original;
    EXPECT_EQ(original.size(), assigned.size());
    EXPECT_EQ(original.find("p1")->value, assigned.find("p1")->value);
}

// ���� �� ��������� ������� ��������
TEST(RBTreeTest, HandlesEmptyPolynomial) {
    RBTree<std::string, Polinom> tree;
    Polinom p("");

    EXPECT_NO_THROW(tree.insert("empty", p));
    EXPECT_EQ(tree.find("empty")->value, Polinom(""));
}

// ���� �� ��������� �������� � ������
TEST(RBTreeTest, HandlesSpacesInKeys) {
    RBTree<std::string, Polinom> tree;
    Polinom p("x + y");

    EXPECT_NO_THROW(tree.insert("id with spaces", p));
    EXPECT_EQ(tree.find("id with spaces")->value, p);
}

// ���� ����������
TEST(RBTreeTest, IteratorsWorkCorrectly) {
    RBTree<int, Polinom> tree;
    Polinom p1("x");
    Polinom p2("y");
    Polinom p3("z");

    tree.insert(3, p3);
    tree.insert(1, p1);
    tree.insert(2, p2);

    std::vector<int> keys;
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        keys.push_back(it->key);
    }

    EXPECT_EQ(keys.size(), 3);
    EXPECT_TRUE(std::is_sorted(keys.begin(), keys.end()));
}

