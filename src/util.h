#ifndef METAL_SCRAPPER_UTIL_H
#define METAL_SCRAPPER_UTIL_H

#include <string>

void message_ok(const char *fmt, ...);

void message_error(const char *fmt, ...);

bool is_float(const std::string &string);

bool is_float_fast(const std::string &string);

#endif //METAL_SCRAPPER_UTIL_H
