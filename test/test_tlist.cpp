#include "tlist.h"
#include <gtest.h>

//LIST TESTS

TEST(tList, can_create_list_without_parameters) {
    ASSERT_NO_THROW(List<int> l);
}


TEST(tList, can_create_list_with_positive_length) {
    ASSERT_NO_THROW(List<int> l(5));
}


TEST(tList, cannot_create_list_with_negative_length) {
    ASSERT_ANY_THROW(List<int> l(-2));
}


TEST(tList, can_create_list_with_large_length) {
    ASSERT_NO_THROW(List<long long> l(9999));
}


TEST(tList, test_assignment_operator_equal_length) {
    List<int> l(2, 0);
    List<int> l1(2, 0);

    l[1] = 1;
    l1 = l;

    EXPECT_EQ(l, l1);
}


TEST(tList, test_assignment_operator_different_length) {
    List<int> l(2, 0);
    List<int> l1(5, 0);

    l[1] = 1;
    l[0] = 23;
    l1 = l;

    EXPECT_EQ(l, l1);
}


TEST(tList, test_assignment_operator_equal_lists) {
    List<int> l(2, 0);
    List<int> l1(2, 0);

    l[1] = 1;
    l1[1] = 1;
    l1 = l;

    EXPECT_EQ(l, l1);
}


TEST(tList, test_assignment_operator_different_memory) {
    List<int> l(2, 0);
    List<int> l1(2, 0);

    l[1] = 1;
    l1 = l;
    l1[1] = 10;

    EXPECT_EQ(1, l[1]);
    EXPECT_EQ(10, l1[1]);
}


TEST(tList, test_index_operator) {
    List<int> l(2, 0);
    List<int> l1(2, 0);

    l[1] = 1;
    l1[1] = l[1];

    EXPECT_EQ(l, l1);
}


TEST(tList, test_index_operator_out_of_range) {
    List<int> l(2, 0);

    ASSERT_ANY_THROW(l[4]);
}


TEST(tList, test_get_size) {
    List<int> l(2);

    int res = l.size();

    EXPECT_EQ(2, res);
}


TEST(tList, test_insert_equal_type) {
    List<int> l(2);
    int sz = l.size();

    l.insert(1, 1);

    EXPECT_EQ(1, l[1]);
    EXPECT_EQ(sz + 1, l.size());
}


TEST(tList, test_insert_front) {
    List<int> l(2);
    int sz = l.size();

    l.insert_front(1);

    EXPECT_EQ(1, l[0]);
    EXPECT_EQ(sz + 1, l.size());
}


TEST(tList, test_erase) {
    List<int> l(3, 0);
    int sz = l.size();

    l[2] = 1;
    l.erase(1);

    EXPECT_EQ(1, l[1]);
    EXPECT_EQ(sz - 1, l.size());
}


TEST(tList, test_erase_front) {
    List<int> l(3, 0);
    int sz = l.size();

    l[0] = 1;
    l.erase_front();

    EXPECT_EQ(0, l[0]);
    EXPECT_EQ(sz - 1, l.size());
}

TEST(tList, test_find_true) {
    List<int> l(3, 0);

    l[2] = 1;

    EXPECT_EQ(2, l.find(1));
}


TEST(tList, test_find_false) {
    List<int> l(3, 0);

    ASSERT_ANY_THROW(l.find(1));
}


TEST(tList, test_get_first) {
    List<int> l(3, 0);

    EXPECT_EQ(0, l.get_first());
}


TEST(tList, test_ostream) {
    List<int> l(2);

    l[1] = 2;

    ASSERT_NO_THROW(std::cout << l);
}

//ITERATOR TESTS

TEST(tListIterator, can_create_iterator_at_beginning) {
    List<int> l(3, 0);
    auto it = l.begin();
    ASSERT_NO_THROW(*it);
}


TEST(tListIterator, can_create_iterator_at_end) {
    List<int> l(3, 0);
    auto it = l.end();
    ASSERT_NO_THROW(it);
}


TEST(tListIterator, can_dereference_iterator) {
    List<int> l(3, 0);
    auto it = l.begin();
    EXPECT_EQ(*it, 0);
}


TEST(tListIterator, can_increment_iterator_prefix) {
    List<int> l(3, 0);
    l[1] = 1;
    auto it = l.begin();
    ++it;
    EXPECT_EQ(*it, 1);
}


TEST(tListIterator, can_increment_iterator_postfix) {
    List<int> l(3, 0);
    l[1] = 1;
    auto it = l.begin();
    it++;
    EXPECT_EQ(*it, 1);
}

TEST(tListIterator, can_compare_iterators_equal) {
    List<int> l(3, 0);
    auto it1 = l.begin();
    auto it2 = l.begin();
    EXPECT_EQ(it1, it2);
}

TEST(tListIterator, can_compare_iterators_not_equal) {
    List<int> l(3, 0);
    auto it1 = l.begin();
    auto it2 = l.end();
    EXPECT_NE(it1, it2);
}

// Тест на работу оператора ->
TEST(tListIterator, can_use_arrow_operator) {
    struct Point {
        int x;
        int y;
    };

    List<Point> l(1, { 1, 2 });
    auto it = l.begin();
    EXPECT_EQ(it->x, 1);
    EXPECT_EQ(it->y, 2);
}


//ZADACHA TESTS

// Тест на нахождение k-ого элемента с конца в списке с несколькими элементами
TEST(tListFindKthFromEnd, can_find_kth_from_end) {
    List<int> l;
    l.append(1);
    l.append(2);
    l.append(3);
    l.append(4);
    l.append(5);

    EXPECT_EQ(l.find_kth_from_end(0), 5); // Последний элемент
    EXPECT_EQ(l.find_kth_from_end(1), 4);
    EXPECT_EQ(l.find_kth_from_end(2), 3);
    EXPECT_EQ(l.find_kth_from_end(3), 2);
    EXPECT_EQ(l.find_kth_from_end(4), 1); // Первый элемент
}

// Тест на нахождение k-ого элемента с конца в списке с одним элементом
TEST(tListFindKthFromEnd, can_find_kth_from_end_single_element) {
    List<int> l;
    l.append(42);

    EXPECT_EQ(l.find_kth_from_end(0), 42);
}

// Тест на нахождение k-ого элемента с конца в пустом списке
TEST(tListFindKthFromEnd, cannot_find_kth_from_end_empty_list) {
    List<int> l;

    ASSERT_ANY_THROW(l.find_kth_from_end(0));
}

// Тест на нахождение k-ого элемента с конца с недопустимым значением k
TEST(tListFindKthFromEnd, cannot_find_kth_from_end_out_of_range) {
    List<int> l;
    l.append(1);
    l.append(2);
    l.append(3);

    ASSERT_ANY_THROW(l.find_kth_from_end(3)); // k больше или равно размеру списка
    ASSERT_ANY_THROW(l.find_kth_from_end(10)); // k сильно превышает размер списка
}

//!!!!!!!!!!!!!!!
//! !Queue tests!
//! !!!!!!!!!!!!!
TEST(QueueTest, CanCreateQueueWithDefaultValues) {
    Queue<int> q;
    EXPECT_EQ(q.getSize(), 0);
    EXPECT_TRUE(q.isEmpty());
}

TEST(QueueTest, CanCreateQueueWithInitialValues) {
    Queue<int> q(3, 5);
    EXPECT_EQ(q.getSize(), 3);
    EXPECT_FALSE(q.isEmpty());
    EXPECT_EQ(q.front(), 5);
}

TEST(QueueTest, CanEnqueueElements) {
    Queue<int> q;
    q.enqueue(10);
    EXPECT_EQ(q.getSize(), 1);
    EXPECT_EQ(q.front(), 10);

    q.enqueue(20);
    EXPECT_EQ(q.getSize(), 2);
    EXPECT_EQ(q.front(), 10);  // Front остается прежним
}

TEST(QueueTest, CanDequeueElements) {
    Queue<int> q;
    q.enqueue(10);
    q.enqueue(20);
    q.dequeue();
    EXPECT_EQ(q.getSize(), 1);
    EXPECT_EQ(q.front(), 20);

    q.dequeue();
    EXPECT_TRUE(q.isEmpty());
    EXPECT_EQ(q.getSize(), 0);
}

TEST(QueueTest, DequeueFromEmptyQueueThrows) {
    Queue<int> q;
    EXPECT_THROW(q.dequeue(), std::underflow_error);
}

TEST(QueueTest, FrontOnEmptyQueueThrows) {
    Queue<int> q;
    EXPECT_THROW(q.front(), std::underflow_error);
}

TEST(QueueTest, CanHandleMultipleOperations) {
    Queue<int> q;
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);
    EXPECT_EQ(q.getSize(), 3);
    EXPECT_EQ(q.front(), 1);

    q.dequeue();
    EXPECT_EQ(q.getSize(), 2);
    EXPECT_EQ(q.front(), 2);

    q.enqueue(4);
    EXPECT_EQ(q.getSize(), 3);
    EXPECT_EQ(q.front(), 2);

    q.dequeue();
    q.dequeue();
    EXPECT_EQ(q.getSize(), 1);
    EXPECT_EQ(q.front(), 4);

    q.dequeue();
    EXPECT_TRUE(q.isEmpty());
    EXPECT_EQ(q.getSize(), 0);
}



