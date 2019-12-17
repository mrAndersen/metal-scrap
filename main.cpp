#include <iostream>
#include <fmt/format.h>
#include <fstream>
#include <regex>


class Collector {
protected:
    std::string procPath;
    std::regex regex;

public:
    std::string readFile(const std::string &path) {
        std::ifstream t(path);
        std::string string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

        return string;
    }
};


class CpuMetrics : public Collector {
public:
    float load1 = 0;
    float load5 = 0;
    float load15 = 0;
    int processCount = 0;
    float parsingTime = 0;

    CpuMetrics() {
        this->regex = std::regex(R"((\d+.\d+)\s(\d+.\d+)\s(\d+.\d+)\s\d+\/(\d+)\s\d+)");
        this->procPath = "/proc/loadavg";
    }

    std::string toString() {
        std::string result = {};

        if (this->load1 == 0) {
            return result;
        }

        auto string = fmt::format(
                "load1={},load5={},load15={},ps={},time={}ms",
                this->load1,
                this->load5,
                this->load15,
                this->processCount,
                this->parsingTime
        );

        return string;
    }

    void collect() {
        auto data = this->readFile(this->procPath);

        std::smatch match;
        std::regex_search(data, match, this->regex);

        this->load1 = std::stof(match[1]);
        this->load5 = std::stof(match[2]);
        this->load15 = std::stof(match[3]);
        this->processCount = std::stoi(match[4]);
    }
};


struct MemoryMetrics {

};


int main() {


    auto cpu = CpuMetrics();
    cpu.collect();

    printf("%s", cpu.toString().c_str());
    return 0;
}
