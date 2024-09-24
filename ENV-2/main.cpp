#include <iostream>
#include "complex.hpp"
#include "sorting.hpp"

int main() {
    Complex arr[] = {Complex(3, 4), Complex(1, 1), Complex(0, 1), Complex(5, 12), Complex(2, 2)};
    int size = sizeof(arr) / sizeof(arr[0]);

    std::cout << "Original array:" << std::endl;
    for (int i = 0; i < size; ++i) {
        std::cout << "(" << arr[i].getReal() << ", " << arr[i].getImag() << ") ";
    }
    std::cout << std::endl;

    sortComplexArray(arr, size);

    std::cout << "Sorted array (by magnitude):" << std::endl;
    for (int i = 0; i < size; ++i) {
        std::cout << "(" << arr[i].getReal() << ", " << arr[i].getImag() << ") ";
    }
    std::cout << std::endl;

    return 0;
}
