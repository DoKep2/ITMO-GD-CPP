//
// Created by sergo on 19.10.2024.
//

#include <iostream>

#include "array/array.h"

int main() {
    Array<int> a;
    for (int i = 0; i < 10; ++i) {
        a.insert(i + 1);
    }

    for (int i = 0; i < a.size(); ++i) {
        a[i] *= 2;
    }
    for (auto it = a.iterator(); it.hasNext(); it.next()) {
        std::cout << it.get() << std::endl;
    }
    return 0;
}