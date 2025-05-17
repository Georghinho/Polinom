#include "unsortedtable.h"
#include <gtest.h>

// Тесты вставки элементов
TEST(DataBaseVector, InsertAddsNewElementAndIncreasesSize) {
    DataBaseVector db;
    Polinom p("x + y + z");

    EXPECT_NO_THROW(db.insert("poly1", p));
    EXPECT_EQ(db.size(), 1);
    EXPECT_EQ(db.find("poly1")->second, p);
}

TEST(DataBaseVector, InsertThrowsForDuplicateId) {
    DataBaseVector db;
    Polinom p1("x^2 + y");
    Polinom p2("y^2 + z");

    db.insert("poly1", p1);
    EXPECT_THROW(db.insert("poly1", p2), std::runtime_error);
}

TEST(DataBaseVector, InsertNoCheckDoesNotValidate) {
    DataBaseVector db;
    Polinom p1("x + y");
    Polinom p2("x^2 + z");

    EXPECT_NO_THROW(db.insert_no_check("poly1", p1));
    EXPECT_NO_THROW(db.insert_no_check("poly1", p2));
    EXPECT_EQ(db.size(), 2);
}

// Тесты удаления элементов
TEST(DataBaseVector, EraseRemovesElementAndDecreasesSize) {
    DataBaseVector db;
    Polinom p("3x + 2y + z");
    db.insert("poly1", p);

    EXPECT_NO_THROW(db.erase("poly1"));
    EXPECT_EQ(db.size(), 0);
    EXPECT_EQ(db.find("poly1"), db.sql.end());
}

TEST(DataBaseVector, EraseThrowsForNonexistentId) {
    DataBaseVector db;
    EXPECT_THROW(db.erase("nonexistent"), std::runtime_error);
}

// Тесты поиска
TEST(DataBaseVector, FindReturnsCorrectIteratorForExistingElement) {
    DataBaseVector db;
    Polinom p("x^3 + y^3 + z^3");
    db.insert("poly1", p);

    auto it = db.find("poly1");
    EXPECT_NE(it, db.sql.end());
    EXPECT_EQ(it->first, "poly1");
    EXPECT_EQ(it->second, p);
}

TEST(DataBaseVector, FindReturnsEndForNonexistentElement) {
    DataBaseVector db;
    EXPECT_EQ(db.find("nonexistent"), db.sql.end());
}

// Тесты операций сравнения
TEST(DataBaseVector, EqualityOperatorForEqualDatabases) {
    DataBaseVector db1, db2;
    Polinom p("x + y");

    db1.insert("p1", p);
    db2.insert("p1", p);

    EXPECT_TRUE(db1 == db2);
}

TEST(DataBaseVector, InequalityOperatorForDifferentDatabases) {
    DataBaseVector db1, db2;
    Polinom p1("x + y");
    Polinom p2("x^2 + y^2");

    db1.insert("p1", p1);
    db2.insert("p1", p2);

    EXPECT_TRUE(db1 != db2);
}

// Тесты работы с несколькими элементами
TEST(DataBaseVector, HandlesMultipleInsertionsAndDeletions) {
    DataBaseVector db;
    Polinom p1("x");
    Polinom p2("y");
    Polinom p3("z");

    db.insert("p1", p1);
    db.insert("p2", p2);
    db.insert("p3", p3);

    EXPECT_EQ(db.size(), 3);

    db.erase("p2");
    EXPECT_EQ(db.size(), 2);
    EXPECT_EQ(db.find("p1")->second, p1);
    EXPECT_EQ(db.find("p3")->second, p3);
}

// Тесты с комплексными полиномами
TEST(DataBaseVector, StoresAndRetrievesComplexPolynomials) {
    DataBaseVector db;
    Polinom p("3.5x^5y^2z - 2.1x^3y^4z^2 + 7.8x^2y^3z^4 - 1.2");

    db.insert("complex", p);
    auto found = db.find("complex");

    ASSERT_NE(found, db.sql.end());
    EXPECT_EQ(found->second, p);
}

// Тесты копирования
TEST(DataBaseVector, CopyConstructorCreatesEqualDatabase) {
    DataBaseVector original;
    Polinom p("x^2 + y^2 + z^2");
    original.insert("p1", p);

    DataBaseVector copy(original);
    EXPECT_EQ(original, copy);
}

TEST(DataBaseVector, AssignmentOperatorCreatesEqualDatabase) {
    DataBaseVector original;
    Polinom p("x^3 + y^3 + z^3");
    original.insert("p1", p);

    DataBaseVector assigned;
    assigned = original;
    EXPECT_EQ(original, assigned);
}

// Тест на обработку пустого полинома
TEST(DataBaseVector, HandlesEmptyPolynomial) {
    DataBaseVector db;
    Polinom p("");

    EXPECT_NO_THROW(db.insert("empty", p));
    EXPECT_EQ(db.find("empty")->second, Polinom(""));
}

// Тест на обработку пробелов в идентификаторах
TEST(DataBaseVector, HandlesSpacesInIds) {
    DataBaseVector db;
    Polinom p("x + y");

    EXPECT_NO_THROW(db.insert("id with spaces", p));
    EXPECT_EQ(db.find("id with spaces")->second, p);
}
