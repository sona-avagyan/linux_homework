#include "shared_array.h"
#include <iostream>

int main() {
    const char* array_name = "/shared_array";
    size_t array_size = 10;

    try {
        shared_array array(array_name, array_size);

        while (true) {
            array.lock();

            // Чтение из массива
            for (size_t i = 0; i < array_size; ++i) {
                std::cout << "Second process read: " << array[i] << " at index " << i << std::endl;
            }

            array.unlock();
            sleep(2);
        }
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}

