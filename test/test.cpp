#include "test.h"

void function_time(void (*function)()) {
    auto start = std::chrono::high_resolution_clock::now();
    function();
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    fmt::print("time {} ns\n", duration.count());
}
