#pragma once

#include <iostream>
#include <cassert>
#include <utility>
#include <vector>

template<typename T>
class Array final {
public:
    static constexpr float kResizeFactor = 2;

    explicit Array(int capacity = 8);

    ~Array();

    Array(const Array& other);

    Array(Array&& other) noexcept;

    Array& operator=(const Array& other);

    Array& operator=(Array&& other) noexcept;

    int insert(const T& value);

    int insert(int index, const T& value);

    void remove(int index);

    const T& operator[](int index) const;
    T& operator[](int index);

    [[nodiscard]] int size() const;
    [[nodiscard]] int capacity() const;

    class Iterator;
    class ConstIterator;
    class ReverseIterator;
    class ConstReverseIterator;

    Iterator iterator();
    ConstIterator constIterator() const;

    ReverseIterator reverseIterator();
    ConstReverseIterator constReverseIterator() const;

 private:
    T* data_;
    int size_;
    int capacity_;

    void resize();
    void clear();
    void swap_(Array& other);

public:
    class Iterator {
    public:
        Iterator(const Array* arr, T* ptr, int size) : array(arr), current(ptr), end(ptr + size), size(size) {}

        const T& get() const {
            validate();
            return *current;
        }

        void set(const T& value) {
            validate();
            *current = value;
        }

        void next() {
            validate();
            ++current;
        }

        [[nodiscard]] bool hasNext() const {
            return current != end;
        }

    private:
        const Array* array;
        T* current{};
        T* end{};
        int size;

        void validate() const {
            if(size != array->size()) {
                throw std::runtime_error("Array was modified during iteration");
            }
        }
    };

    class ConstIterator {
    public:
        ConstIterator(const T* ptr, int size) : current(ptr), end(ptr + size) {}

        const T& get() const {
            return *current;
        }

        void next() {
            ++current;
        }

        [[nodiscard]] bool hasNext() const {
            return current != end;
        }

    private:
        const T* current;
        const T* end;
    };

    class ReverseIterator {
    public:
        ReverseIterator(T* ptr, int size) : current(ptr + size - 1), start(ptr) {}

        const T& get() const {
            return *current;
        }

        void set(const T& value) {
            *current = value;
        }

        void next() {
            --current;
        }

        [[nodiscard]] bool hasNext() const {
            return current != start;
        }

    private:
        T* current;
        T* start;
    };

    class ConstReverseIterator {
    public:
        ConstReverseIterator(const T* ptr, int size) : current(ptr + size - 1), start(ptr) {}

        const T& get() const {
            return *current;
        }

        void next() {
            --current;
        }

        [[nodiscard]] bool hasNext() const {
            return current != start;
        }
    private:
        const T* current;
        const T* start;
    };
};


template<typename T>
Array<T>::Array(const int capacity) : size_(0), capacity_(capacity) {
    data_ = static_cast<T*>(malloc(capacity_ * sizeof(T)));
}

template<typename T>
Array<T>::~Array() {
    clear();
    free(data_);
}

// copy constructor
template<typename T>
Array<T>::Array(const Array& other) : size_(other.size_), capacity_(other.capacity_) {
    data_ = static_cast<T*>(malloc(capacity_ * sizeof(T)));
    for(int i = 0; i < size_; i++) {
        new (&data_[i]) T(other.data_[i]);
    }
}


// move constructor
template<typename T>
Array<T>::Array(Array&& other) noexcept : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.size_ = 0;
    other.capacity_ = 0;
    other.data_ = nullptr;
}

template<typename T>
Array<T>& Array<T>::operator=(const Array& other) {
    Array tmp(other);
    swap_(tmp);
    return *this;
}

// move assignment
template<typename T>
Array<T>& Array<T>::operator=(Array&& other) noexcept {
    swap_(other);
    return *this;
}

template<typename T>
int Array<T>::insert(const T& value) {
    return insert(size_, value);
}

template<typename T>
int Array<T>::insert(int index, const T& value) {
    assert(index >= 0 && index <= size_);
    if (size_ >= capacity_) {
        resize();
    }
    for (int i = size_; i > index; --i) {
        new (&data_[i]) T(std::move(data_[i - 1]));
        data_[i - 1].~T();
    }
    new (&data_[index]) T(value);
    ++size_;
    return index;
}

template<typename T>
void Array<T>::remove(int index) {
    assert(index >= 0 && index < size_);
    data_[index].~T();
    for (int i = index; i < size_ - 1; ++i) {
        new (&data_[i]) T(std::move(data_[i + 1]));
        data_[i + 1].~T();
    }
    --size_;
}

template<typename T>
const T& Array<T>::operator[](int index) const {
    return data_[index];
}

template<typename T>
T& Array<T>::operator[](int index) {
    return data_[index];
}

template<typename T>
int Array<T>::size() const {
    return size_;
}

template<typename T>
int Array<T>::capacity() const {
    return capacity_;
}

template<typename T>
typename Array<T>::Iterator Array<T>::iterator() {
    return Iterator(this, data_, size_);
}

template<typename T>
typename Array<T>::ConstIterator Array<T>::constIterator() const {
    return ConstIterator(data_, size_);
}

template<typename T>
typename Array<T>::ReverseIterator Array<T>::reverseIterator() {
    return ReverseIterator(data_, size_);
}

template<typename  T>
typename Array<T>::ConstReverseIterator Array<T>::constReverseIterator() const {
    return ConstReverseIterator(data_, size_);
}


template<typename T>
void Array<T>::resize() {
    const int new_capacity = static_cast<int>(capacity_ * kResizeFactor);
    T* new_data = static_cast<T*>(malloc(new_capacity * sizeof(T)));
    for(size_t i = 0; i < size_; i++) {
        new (&new_data[i]) T(std::move(data_[i]));
        data_[i].~T();
    }
    free(data_);
    data_ = new_data;
    capacity_ = new_capacity;
}

template<typename T>
void Array<T>::clear() {
    for (int i = 0; i < size_; ++i) {
        data_[i].~T();
    }
}
template<typename T>
void Array<T>::swap_(Array& other) {
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(data_, other.data_);
}