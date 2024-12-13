#include "array/array.h"

#include <iostream>
#include <cassert>
#include <cstring>
#include <utility>

template<typename T>
Array<T>::Array(const int capacity) : size_(0), capacity_(capacity) {
    data_ = static_cast<T*>(malloc(capacity_ * sizeof(T)));
}

template<typename T>
Array<T>::~Array() {
    clear();
    free(data_);
}

template<typename T>
Array<T>::Array(const Array& other) : size_(other.size_), capacity_(other.capacity_) {
    data_ = static_cast<T*>(malloc(capacity_ * sizeof(T)));
    std::memcpy(data_, other.data_, size_ * sizeof(T));
}

template<typename T>
Array<T>::Array(Array&& other) noexcept : size_(other.size_), capacity_(other.capacity_), data_(other.data_) {
    other.size_ = 0;
    other.capacity_ = 0;
    other.data_ = nullptr;
}

template<typename T>
Array<T>& Array<T>::operator=(const Array& other) {
    if (this == &other) return *this;
    clear();
    size_ = other.size_;
    capacity_ = other.capacity_;
    data_ = static_cast<T*>(malloc(capacity_ * sizeof(T)));
    std::memcpy(data_, other.data_, size_ * sizeof(T));
    return *this;
}

template<typename T>
Array<T>& Array<T>::operator=(Array&& other) noexcept {
    if (this == &other) return *this;
    clear();
    size_ = other.size_;
    capacity_ = other.capacity_;
    data_ = other.data_;
    other.size_ = 0;
    other.capacity_ = 0;
    other.data_ = nullptr;
    return *this;
}

template<typename T>
int Array<T>::insert(const T& value) {
    return insert(size_, value);
}

template<typename T>
int Array<T>::insert(int index, const T& value) {
    assert(index >= 0 && index <= size_);   // index should be valid
    if (size_ >= capacity_) {
        resize();
    }
    for (int i = size_; i > index; --i) {
        new (&data_[i]) T(std::move(data_[i - 1]));   // Move existing elements
        data_[i - 1].~T();  // Call destructor for moved-from elements
    }
    new (&data_[index]) T(value);  // Construct the new element
    ++size_;
    return index;
}

// Удаление элементов
template<typename T>
void Array<T>::remove(int index) {
    assert(index >= 0 && index < size_);  // index should be valid
    data_[index].~T();  // Call destructor
    for (int i = index; i < size_ - 1; ++i) {
        new (&data_[i]) T(std::move(data_[i + 1]));  // Move elements left
        data_[i + 1].~T();  // Call destructor for moved-from elements
    }
    --size_;
}

// Оператор доступа
template<typename T>
const T& Array<T>::operator[](int index) const {
    return data_[index];  // No bounds checking as per requirement
}

template<typename T>
T& Array<T>::operator[](int index) {
    return data_[index];  // No bounds checking as per requirement
}

// Метод получения текущего размера
template<typename T>
int Array<T>::size() const {
    return size_;
}

// Итераторы
template<typename T>
typename Array<T>::Iterator Array<T>::iterator() {
    return Iterator(data_);
}

template<typename T>
typename Array<T>::Iterator Array<T>::begin() {
    return Iterator(data_);
}

template<typename T>
typename Array<T>::Iterator Array<T>::end() {
    return Iterator(data_ + size_);
}

template<typename T>
typename Array<T>::Iterator Array<T>::reverseIterator() {
    return Iterator(data_ + size_ - 1);
}

// Приватные методы
template<typename T>
void Array<T>::resize() {
    int new_capacity = static_cast<int>(capacity_ * 1.6);
    T* new_data = static_cast<T*>(malloc(new_capacity * sizeof(T)));
    std::memcpy(new_data, data_, size_ * sizeof(T));
    free(data_);
    data_ = new_data;
    capacity_ = new_capacity;
}

template<typename T>
void Array<T>::clear() {
    for (int i = 0; i < size_; ++i) {
        data_[i].~T();  // Call destructor for each element
    }
    size_ = 0;
}

template<typename T>
Array<T>::Iterator::Iterator(T* ptr) : current(ptr) {}

template<typename T>
const T& Array<T>::Iterator::get() const {
    return *current;
}

template<typename T>
void Array<T>::Iterator::set(const T& value) {
    *current = value;
}

template<typename T>
void Array<T>::Iterator::next() {
    ++current;
}

template<typename T>
bool Array<T>::Iterator::hasNext() const {
    return current != nullptr;
}

template<typename T>
T& Array<T>::Iterator::operator*() const {
    return *current;
}

template<typename T>
typename Array<T>::Iterator& Array<T>::Iterator::operator++() {
    ++current;
    return *this;
}

template<typename T>
bool Array<T>::Iterator::operator!=(const Iterator& other) const {
    return current != other.current;
}

