// //
// // Created by sergo on 19.10.2024.
// //

#include <gtest/gtest.h>

#include <utility>

#include "array/array.h"

// Test default constructor
TEST(ArrayTest, DefaultConstructor) {
    Array<int> arr;
    EXPECT_EQ(arr.size(), 0);
    EXPECT_EQ(arr.capacity(), 8);
}

// Test constructor with capacity
TEST(ArrayTest, ConstructorWithCapacity) {
    const Array<int> arr(20);
    EXPECT_EQ(arr.size(), 0);
    EXPECT_EQ(arr.capacity(), 20);
}

// Test insert and size
TEST(ArrayTest, InsertAndSize) {
    Array<int> arr;
    arr.insert(42);
    EXPECT_EQ(arr.size(), 1);
    EXPECT_EQ(arr[0], 42);
}

// Test insert at specific index
TEST(ArrayTest, InsertAtIndex) {
    Array<int> arr;
    arr.insert(42);
    arr.insert(0, 24);
    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0], 24);
    EXPECT_EQ(arr[1], 42);
}

// Test remove
TEST(ArrayTest, Remove) {
    Array<int> arr;
    arr.insert(42);
    arr.insert(24);
    arr.remove(0);
    EXPECT_EQ(arr.size(), 1);
    EXPECT_EQ(arr[0], 24);
}

// Test indexing
TEST(ArrayTest, Indexing) {
    Array<int> arr;
    arr.insert(42);
    EXPECT_EQ(arr[0], 42);
    arr[0] = 24;
    EXPECT_EQ(arr[0], 24);
}

// Test copy constructor
TEST(ArrayTest, CopyConstructor) {
    Array<int> arr;
    arr.insert(42);
    Array<int> arrCopy = arr;
    EXPECT_EQ(arrCopy.size(), 1);
    EXPECT_EQ(arrCopy[0], 42);
}

// Test move constructor
TEST(ArrayTest, MoveConstructor) {
    Array<int> arr;
    arr.insert(42);
    Array<int> arrMoved = std::move(arr);
    EXPECT_EQ(arrMoved.size(), 1);
    EXPECT_EQ(arrMoved[0], 42);
    EXPECT_EQ(arr.size(), 0); // Original array should be empty
}

// Test assignment operator
TEST(ArrayTest, AssignmentOperator) {
    Array<int> arr;
    arr.insert(42);
    Array<int> arrCopy = arr;
    EXPECT_EQ(arrCopy.size(), 1);
    EXPECT_EQ(arrCopy[0], 42);
}

// Test move assignment operator
TEST(ArrayTest, MoveAssignmentOperator) {
    Array<int> arr;
    arr.insert(42);
    Array<int> arrMoved = std::move(arr);
    EXPECT_EQ(arrMoved.size(), 1);
    EXPECT_EQ(arrMoved[0], 42);
    EXPECT_EQ(arr.size(), 0); // Original array should be empty
}

// Test iterator
TEST(ArrayTest, Iterator) {
    Array<int> arr;
    for (int i = 0; i < 5; ++i) {
        arr.insert(i);
    }
    auto it = arr.iterator();
    for (int i = 0; i < 5; ++i) {
        ASSERT_TRUE(it.hasNext());
        EXPECT_EQ(it.get(), i);
        it.next();
    }
    EXPECT_FALSE(it.hasNext());
}

// Test iterator after array modification
TEST(ArrayTest, IteratorAfterModification) {
    Array<int> arr;
    for (int i = 0; i < 5; ++i) {
        arr.insert(i);
    }
    const auto it = arr.iterator();
    arr.insert(42);
    ASSERT_THROW(it.get(), std::runtime_error);
}

// Test reverse iterator
TEST(ArrayTest, ReverseIterator) {
    Array<int> arr;
    for (int i = 0; i < 5; ++i) {
        arr.insert(i);
    }
    auto it = arr.reverseIterator();
    for (int i = 4; i > 0; --i) {
        ASSERT_TRUE(it.hasNext());
        EXPECT_EQ(it.get(), i);
        it.next();
    }
    EXPECT_FALSE(it.hasNext());
}

// Test with custom type
struct TestStruct {
    std::string s;
    explicit TestStruct(std::string  val) : s(std::move(val)) {}
    bool operator==(const TestStruct& other) const {
        return s == other.s;
    }
};

TEST(ArrayTest, InsertCustomType) {
    Array<TestStruct> arr;
    arr.insert(TestStruct("abacaba"));
    EXPECT_EQ(arr.size(), 1);
    EXPECT_EQ(arr[0].s, "abacaba");
}

TEST(ArrayTest, RemoveCustomType) {
    Array<TestStruct> arr;
    arr.insert(TestStruct("abacaba"));
    arr.remove(0);
    EXPECT_EQ(arr.size(), 0);
}

TEST(ArrayTest, IndexingCustomType) {
    Array<TestStruct> arr;
    arr.insert(TestStruct("abacaba"));
    EXPECT_EQ(arr[0].s, "abacaba");
    arr[0].s = "qwerty";
    EXPECT_EQ(arr[0].s, "qwerty");
}

TEST(ArrayTest, CopyConstructorCustomType) {
    Array<TestStruct> arr;
    arr.insert(TestStruct("abacaba"));
    Array<TestStruct> arrCopy = arr;
    EXPECT_EQ(arrCopy.size(), 1);
    EXPECT_EQ(arrCopy[0].s, "abacaba");
}

TEST(ArrayTest, MoveConstructorCustomType) {
    Array<TestStruct> arr;
    arr.insert(TestStruct("abacaba"));
    Array<TestStruct> arrMoved = std::move(arr);
    EXPECT_EQ(arrMoved.size(), 1);
    EXPECT_EQ(arrMoved[0].s, "abacaba");
    EXPECT_EQ(arr.size(), 0); // Original array should be empty
}

TEST(ArrayTest, AssignmentOperatorCustomType) {
    Array<TestStruct> arr;
    arr.insert(TestStruct("abacaba"));
    Array<TestStruct> arrCopy = arr;
    EXPECT_EQ(arrCopy.size(), 1);
    EXPECT_EQ(arrCopy[0].s, "abacaba");
}

TEST(ArrayTest, MoveAssignmentOperatorCustomType) {
    Array<TestStruct> arr;
    arr.insert(TestStruct("abacaba"));
    Array<TestStruct> arrMoved = std::move(arr);
    EXPECT_EQ(arrMoved.size(), 1);
    EXPECT_EQ(arrMoved[0].s, "abacaba");
    EXPECT_EQ(arr.size(), 0); // Original array should be empty
}

// Test resizing
TEST(ArrayTest, Resize) {
    Array<int> arr(2);
    arr.insert(1);
    arr.insert(2);
    arr.insert(3);
    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr.capacity(), 4);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
}
