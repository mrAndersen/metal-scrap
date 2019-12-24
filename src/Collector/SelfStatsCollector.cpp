#ifndef METAL_SCRAPPER_SelfStatsCollector_CPP
#define METAL_SCRAPPER_SelfStatsCollector_CPP

#include "Collector.h"
#include "../Node/SelfStatsNode.cpp"

class SelfStatsCollector : public Collector {
public:
    SelfStatsCollector(Master *master) : Collector(master) {
        this->regex = std::regex(R"(.*VmRSS:\s+(\d+).*)");
        this->procPath = master->procLocation + "/self/status";

        master->addCollector(this);
    }

    void collect() override {
        auto data = read_file(this->procPath);

        std::smatch match;
        std::regex_search(data, match, this->regex);

        auto node = new SelfStatsNode();
        node->bytesUsed = std::stol(match[1]) * 1024;

        this->collected.emplace_back(node);
    }

    std::string getName() override {
        return "collector_self_stats";
    }

    int getCollectPeriodMs() override {
        return 1000;
    }
};

#endif