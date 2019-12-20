#include "test.h"

void function_time(void (*function)()) {
    auto start = std::chrono::high_resolution_clock::now();
    function();
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    fmt::print("time {} ns\n", duration.count());
}

void unit_tests() {
    do_assert(!is_float("1,22"));
    do_assert(is_float("1.22"));
    do_assert(is_float("1.223321312"));
    do_assert(!is_float("1,223321312"));
    do_assert(is_float("1"));
    do_assert(is_float("2"));
    do_assert(is_float("0"));
    do_assert(!is_float("test"));
    do_assert(!is_float("test2"));
    do_assert(!is_float("2test"));
    do_assert(!is_float("t3est"));
}
