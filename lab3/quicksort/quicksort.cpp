#include "quicksort.h"

#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <utility>

int INSERTION_SORT_THRESHOLD = 16;

template <typename T, typename Compare>
void insertion_sort(T* first, T* last, Compare comp) {
    for (T* i = first + 1; i < last; ++i) {
        T tmp = std::move(*i);
        T* j = i;
        while (j > first && comp(tmp, *(j - 1))) {
            *j = std::move(*(j - 1));
            --j;
        }
        *j = std::move(tmp);
    }
}

template <typename T, typename Compare>
T* median_of_three(T* a, T* b, T* c, Compare comp) {
    return comp(*a, *b) ? (comp(*b, *c) ? b : (comp(*a, *c) ? c : a))
                        : (comp(*a, *c) ? a : (comp(*b, *c) ? c : b));
}

template <typename T, typename Compare>
void quicksort(T* first, T* last, Compare comp) {
    while (last - first > INSERTION_SORT_THRESHOLD) {
        T* pivot = median_of_three(first, first + (last - first) / 2, last - 1, comp);
        std::swap(*pivot, *(last - 1));
        pivot = last - 1;

        T* i = first;
        T* j = last - 2;

        while (true) {
            while (comp(*i, *pivot)) ++i;
            while (comp(*pivot, *j)) --j;
            if (i >= j) break;
            std::swap(*i, *j);
            ++i;
            --j;
        }
        std::swap(*i, *pivot);

        if (i - first < last - (i + 1)) {
            quicksort(first, i, comp);
            first = i + 1;
        } else {
            quicksort(i + 1, last, comp);
            last = i;
        }
    }

    insertion_sort(first, last, comp);
}

template <typename T, typename Compare>
void sort(T* first, T* last, Compare comp) {
    quicksort(first, last, comp);
}

template <typename T, typename Compare>
void quicksort_no_insertion(T* first, T* last, Compare comp) {
    if (last - first <= 1) return;

    T* pivot = first + (last - first) / 2;
    std::swap(*pivot, *(last - 1));
    pivot = last - 1;

    T* i = first;
    T* j = last - 2;

    while (true) {
        while (comp(*i, *pivot)) ++i;
        while (comp(*pivot, *j)) --j;
        if (i >= j) break;
        std::swap(*i, *j);
        ++i;
        --j;
    }
    std::swap(*i, *pivot);

    quicksort_no_insertion(first, i, comp);
    quicksort_no_insertion(i + 1, last, comp);
}


template <typename Func>
double measure_time(Func sort_function, std::vector<int> data) {
    auto start = std::chrono::high_resolution_clock::now();
    sort_function(data);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

// Генерация массивов
std::vector<int> generate_random_data(size_t size) {
    std::vector<int> data(size);
    for (auto& x : data) x = rand() % 10000;
    return data;
}

std::vector<int> generate_reverse_sorted_data(size_t size) {
    std::vector<int> data(size);
    for (size_t i = 0; i < size; ++i) data[i] = size - i;
    return data;
}


// int main() {
//     std::vector<size_t> sizes;
//     for(size_t i = 1; i <= 100; ++i) {
//         sizes.push_back(i);
//     }
//     std::vector<double> quick_random_times, quick_reverse_times;
//     std::vector<double> insert_random_times, insert_reverse_times;
//
//     for (size_t size : sizes) {
//         auto random_data = generate_random_data(size);
//         auto reverse_data = generate_reverse_sorted_data(size);
//
//         quick_random_times.push_back(
//             measure_time([&](std::vector<int> data) { quicksort(data.data(), data.data() + data.size(), std::less<int>()); }, random_data));
//         quick_reverse_times.push_back(
//             measure_time([&](std::vector<int> data) { quicksort(data.data(), data.data() + data.size(), std::less<int>()); }, reverse_data));
//         insert_random_times.push_back(
//             measure_time([&](std::vector<int> data) { insertion_sort(data.data(), data.data() + data.size(), std::less<int>()); }, random_data));
//         insert_reverse_times.push_back(
//             measure_time([&](std::vector<int> data) { insertion_sort(data.data(), data.data() + data.size(), std::less<int>()); }, reverse_data));
//     }
//
//     std::cout << "Size, QuickSort(Random), InsertionSort(Random), QuickSort(Reverse), InsertionSort(Reverse)\n";
//     for (size_t i = 0; i < sizes.size(); ++i) {
//         std::cout << sizes[i] << ", " << quick_random_times[i] << ", " << insert_random_times[i] << ", "
//                   << quick_reverse_times[i] << ", " << insert_reverse_times[i] << "\n";
//     }
//
//     return 0;
// }

int main() {
    std::vector<size_t> sizes = {2, 4, 8, 16, 32, 64, 100, 1000, 10000};
    // for(size_t i = 1; i <= 100; ++i) {
    // sizes.push_back(i);
    // }
    std::vector<double> quick_with_insertion, quick_without_insertion;
    for(size_t size: sizes) {
        auto random_data = generate_random_data(size);
        quick_with_insertion.push_back(measure_time([&](std::vector<int> data) { sort(data.data(), data.data() + data.size(), std::less<int>()); }, random_data));
        quick_without_insertion.push_back(measure_time([&](std::vector<int> data) { quicksort_no_insertion(data.data(), data.data() + data.size(), std::less<int>()); }, random_data));
    }

    std::cout << "Size, QuickSort with Insertion, QuickSort without Insertion\n";
    for(size_t i = 0; i < sizes.size(); ++i) {
        std::cout << sizes[i] << ", " << quick_with_insertion[i] << ", " << quick_without_insertion[i] << "\n";
    }

}

template void sort<int, std::function<bool(const int&, const int&)>>(int*, int*, std::function<bool(const int&, const int&)>);
template void sort<double, std::function<bool(const double&, const double&)>>(double*, double*, std::function<bool(const double&, const double&)>);
template void sort<std::string, std::function<bool(const std::string&, const std::string&)>>(std::string*, std::string*, std::function<bool(const std::string&, const std::string&)>);
template void sort<std::vector<int>, std::function<bool(const std::vector<int>&, const std::vector<int>&)>>(std::vector<int>*, std::vector<int>*, std::function<bool(const std::vector<int>&, const std::vector<int>&)>);

