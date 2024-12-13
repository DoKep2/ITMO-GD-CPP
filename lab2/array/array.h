#pragma once

template<typename T>
class Array final {
 public:
    class Iterator {
     public:
        explicit Iterator(T* ptr);

        const T& get() const;
        void set(const T& value);
        void next();
        [[nodiscard]] bool hasNext() const;

        T& operator*() const;

        Iterator& operator++();

        bool operator!=(const Iterator& other) const;

     private:
        T* current;
    };

    explicit Array(int capacity = 8);

    ~Array();

    // Copy constructor
    Array(const Array& other);

    // Move constructor
    Array(Array&& other) noexcept;

    // Copy assignment operator
    Array& operator=(const Array& other);

    // Move assignment operator
    Array& operator=(Array&& other) noexcept;

    int insert(const T& value);

    int insert(int index, const T& value);

    void remove(int index);

    const T& operator[](int index) const;
    T& operator[](int index);

    [[nodiscard]] int size() const;

    Iterator iterator();
    Iterator begin();
    Iterator end();
    Iterator reverseIterator();

 private:
    T* data_;
    int size_;
    int capacity_;

    void resize();
    void clear();
};

template class Array<int>; // Явная инстанциация для int