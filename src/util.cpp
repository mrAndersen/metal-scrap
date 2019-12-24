#include "util.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/**
 *
 * @param source
 * @param delimiter
 * @return
 */
std::vector<std::string> explode(const std::string &source, const char &delimiter) {
    auto result = std::vector<std::string>();
    auto buffer = std::string();

    for (int i = 0; i < source.size(); ++i) {
        auto c = source[i];

        if (c == delimiter) {
            result.emplace_back(buffer);
            buffer = "";
        } else {
            buffer += c;
        }
    }

    if (!buffer.empty()) {
        result.emplace_back(buffer);
    }

    return result;
}

/**
 *
 * @param source
 * @param delimiter
 * @return
 */
std::string implode(const std::vector<std::string> &source, const std::string &delimiter) {
    auto result = std::string();

    for (int i = 0; i < source.size(); ++i) {
        result += source[i];

        if (i != source.size() - 1) {
            result += delimiter;
        }
    }

    return result;
}


std::string read_file(const std::string &path) {
    std::ifstream t(path);
    std::string string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    return string;
}

/**
 *
 * @param format
 * @param ...
 */
void message_error(const char *fmt, ...) {
    auto now = std::time(nullptr);
    auto cpp_format = std::string(fmt);

    char date[100];
    std::strftime(date, sizeof(date), "%Y-%m-%d %H:%I:%S", std::localtime(&now));

    cpp_format = fmt::format("[{:s}] {:s}[-]{:s} ", date, ANSI_COLOR_RED, ANSI_COLOR_RESET) + cpp_format + "\n";

    va_list args;
    va_start(args, fmt);

    vfprintf(stderr, cpp_format.c_str(), args);
    va_end(args);
}

/**
 *
 * @param format
 * @param ...
 */
void message_ok(const char *fmt, ...) {
    auto now = std::time(nullptr);
    auto cpp_format = std::string(fmt);

    char date[100];
    std::strftime(date, sizeof(date), "%Y-%m-%d %H:%I:%S", std::localtime(&now));

    cpp_format = fmt::format("[{:s}] {:s}[+]{:s} ", date, ANSI_COLOR_GREEN, ANSI_COLOR_RESET) + cpp_format + "\n";

    va_list args;
    va_start(args, fmt);

    vfprintf(stderr, cpp_format.c_str(), args);

    va_end(args);
}

bool is_float(const std::string &string) {
    for (auto &c:string) {
        if (c == 44 || c == 47) {
            return false;
        }

        if ((c < 46 || c > 57)) {
            return false;
        }
    }

    return true;
}

bool file_exists(const std::string &path) {
    struct stat buffer = {};
    return (stat(path.c_str(), &buffer) == 0);
}
