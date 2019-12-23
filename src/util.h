#ifndef METAL_SCRAPPER_UTIL_H
#define METAL_SCRAPPER_UTIL_H

#include <string>
#include <fmt/format.h>
#include <ctime>
#include <cstdarg>
#include <regex>
#include <fstream>
#include <sys/stat.h>


#define START_TIME auto startTime = std::chrono::high_resolution_clock::now()

#define END_TIME_NS std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - startTime).count()
#define END_TIME_MS std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime).count()
#define END_TIME_S std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - startTime).count()



std::string read_file(const std::string &path);

std::vector<std::string> explode(const std::string &source, const char &delimiter);

std::string implode(const std::vector<std::string> &source, const std::string &delimiter);

bool file_exists(const std::string &path);

void message_ok(const char *fmt, ...);

void message_error(const char *fmt, ...);

bool is_float(const std::string &string);

#endif //METAL_SCRAPPER_UTIL_H
