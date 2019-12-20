#ifndef METAL_SCRAPPER_MemoryCollector_CPP
#define METAL_SCRAPPER_MemoryCollector_CPP

#include "Collector.h"
#include <iostream>
#include <sys/statvfs.h>
#include "../Node/MemoryNode.cpp"

class MemoryCollector : public Collector {
public:
    MemoryCollector() {
        this->regex = std::regex(R"(.*?MemTotal:\s+(\d+).*?MemAvailable:\s+(\d+))");
        this->procPath = "/proc/meminfo";
    }

    void collect() override {
        auto data = read_file(this->procPath);

        std::smatch match;
        std::regex_search(data, match, this->regex);

        auto n = match[1].str();
        auto n2 = match[2].str();

        auto node = new MemoryNode();
        node->totalBytes = std::stol(match[1]) * 1024;
        node->freeBytes = std::stol(match[2]) * 1024;

        this->collected.emplace_back(node);
    }

    std::string getName() override {
        return "collector_memory";
    }

    int getCollectPeriodMs() override {
        return 1000;
    }
};

#endif