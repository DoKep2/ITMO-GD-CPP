//
// Created by sergo on 03.11.2024.
//

#pragma once

template <typename T, typename Compare>
void insertion_sort(T* first, T* last, Compare comp);

template <typename T, typename Compare>
T* median_of_three(T* a, T* b, T* c, Compare comp);

template <typename T, typename Compare>
void quicksort(T* first, T* last, Compare comp);

template <typename T, typename Compare>
void sort(T* first, T* last, Compare comp);
