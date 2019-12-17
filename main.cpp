#include <iostream>
#include <fmt/format.h>
#include <fstream>
#include <regex>


class CpuMetrics {
public:
    float load1 = 0;
    float load5 = 0;
    float load15 = 0;
    int processCount = 0;
    float parsingTime = 0;

    std::string toString() {
        auto string = fmt::format(
                "load1={},load5={},load15={},ps={},time={}ms",
                this->load1,
                this->load5,
                this->load15,
                this->processCount
        );

        return string;
    }
};

class Collector {
protected:
    std::string procPath = "/proc";
    std::regex cpu_r;

public:
    Collector() {
        this->cpu_r = R"((\d+.\d+)\s(\d+.\d+)\s(\d+.\d+)\s\d+\/(\d+)\s\d+)";

    }

public:
    std::string readFile(const std::string &path) {
        std::ifstream t(path);
        std::string string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

        return string;
    }


    CpuMetrics collectCpu() {
        auto path = this->procPath + "/loadavg";
        auto data = this->readFile(path);

        std::smatch match;
        std::regex_search(data, match, this->cpu_r);

        CpuMetrics cpuMetrics;
        cpuMetrics.load1 = std::stof(match[1]);
        cpuMetrics.load5 = std::stof(match[2]);
        cpuMetrics.load15 = std::stof(match[3]);
        cpuMetrics.processCount = std::stoi(match[4]);

        return cpuMetrics;
    }
};

struct MemoryMetrics {

};


int main() {
    std::string procPath = "/proc";
    Collector collector;


    auto cpu = collector.collectCpu();
    printf(cpu.toString().c_str());


    return 0;
}
