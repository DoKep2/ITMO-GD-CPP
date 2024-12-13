//
// Created by sergo on 19.10.2024.
//

#include <gtest/gtest.h>

#include "array/array.h"

// Тест конструктора и метода size()
TEST(ArrayTest, ConstructorAndSize) {
    const Array<int> arr;
    EXPECT_EQ(arr.size(), 0); // Проверяем, что размер по умолчанию 0

    const Array<int> arr_with_capacity(20);
    EXPECT_EQ(arr_with_capacity.size(), 0); // Проверяем, что начальный размер массива 0, даже при установленной capacity
}

// Тест вставки элементов в конец массива
TEST(ArrayTest, InsertEnd) {
    Array<int> arr;
    arr.insert(10);
    arr.insert(20);
    arr.insert(30);

    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
    EXPECT_EQ(arr[2], 30);
}

// Тест вставки элементов в определённую позицию
TEST(ArrayTest, InsertAtIndex) {
    Array<int> arr;
    arr.insert(10);
    arr.insert(20);
    arr.insert(0, 5); // Вставляем в начало
    arr.insert(2, 15); // Вставляем между

    EXPECT_EQ(arr.size(), 4);
    EXPECT_EQ(arr[0], 5);   // Проверяем, что 5 вставилось в начало
    EXPECT_EQ(arr[1], 10);  // Остальные элементы должны сместиться
    EXPECT_EQ(arr[2], 15);
    EXPECT_EQ(arr[3], 20);
}

// Тест удаления элементов
TEST(ArrayTest, Remove) {
    Array<int> arr;
    arr.insert(10);
    arr.insert(20);
    arr.insert(30);
    arr.remove(1); // Удаляем элемент с индексом 1 (20)

    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0], 10);  // Проверяем, что элементы сместились
    EXPECT_EQ(arr[1], 30);
}

// Тест копирования массива
TEST(ArrayTest, CopyConstructor) {
    Array<int> arr;
    arr.insert(10);
    arr.insert(20);

    Array<int> arr_copy(arr);
    EXPECT_EQ(arr_copy.size(), 2);
    EXPECT_EQ(arr_copy[0], 10);
    EXPECT_EQ(arr_copy[1], 20);
}

// Тест перемещения массива
TEST(ArrayTest, MoveConstructor) {
    Array<int> arr;
    arr.insert(10);
    arr.insert(20);

    Array<int> arr_move(std::move(arr));
    EXPECT_EQ(arr_move.size(), 2);
    EXPECT_EQ(arr_move[0], 10);
    EXPECT_EQ(arr_move[1], 20);
    EXPECT_EQ(arr.size(), 0); // Исходный массив должен быть пустым после перемещения
}

// Тест итератора
TEST(ArrayTest, Iterator) {
    Array<int> arr;
    for (int i = 1; i <= 5; ++i) {
        arr.insert(i);
    }

    auto it = arr.iterator();
    int expected_value = 1;
    while (it.hasNext()) {
        EXPECT_EQ(it.get(), expected_value);
        it.next();
        ++expected_value;
    }
}

// Тест обратного итератора
TEST(ArrayTest, ReverseIterator) {
    Array<int> arr;
    for (int i = 1; i <= 5; ++i) {
        arr.insert(i);
    }

    auto it = arr.reverseIterator();
    int expected_value = 5;
    while (it.hasNext()) {
        EXPECT_EQ(it.get(), expected_value);
        it.next();
        --expected_value;
    }
}

// Тест оператора индексирования
TEST(ArrayTest, AccessOperator) {
    Array<int> arr;
    arr.insert(10);
    arr.insert(20);

    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);

    arr[0] = 15;
    EXPECT_EQ(arr[0], 15);
}

