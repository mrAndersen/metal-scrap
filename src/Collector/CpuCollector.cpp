#ifndef METAL_SCRAPPER_CpuCollector_CPP
#define METAL_SCRAPPER_CpuCollector_CPP

#include "Collector.h"
#include "../Node/CpuNode.cpp"

class CpuCollector : public Collector {
public:
    CpuCollector() {
        this->regex = std::regex(R"((\d+.\d+)\s(\d+.\d+)\s(\d+.\d+)\s\d+\/(\d+)\s\d+)");
        this->procPath = "/proc/loadavg";
    }

    void collect() override {
        auto data = read_file(this->procPath);

        std::smatch match;
        std::regex_search(data, match, this->regex);

        auto node = new CpuNode();
        node->load1 = std::stof(match[1]);
        node->load5 = std::stof(match[2]);
        node->load15 = std::stof(match[3]);
        node->processCount = std::stoi(match[4]);

        this->collected.emplace_back(node);
    }

    std::string getName() override {
        return "collector_cpu";
    }

    int getCollectPeriodMs() override {
        return 1000;
    }
};

#endif