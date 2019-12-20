#ifndef METAL_SCRAPPER_TEST_H
#define METAL_SCRAPPER_TEST_H

#include <chrono>
#include <fmt/printf.h>
#include "../src/util.h"
#include <cassert>

#define do_assert(function) (assert(function), message_ok(#function))

void function_time(void (*function)());

void unit_tests();


#endif //METAL_SCRAPPER_TEST_H
