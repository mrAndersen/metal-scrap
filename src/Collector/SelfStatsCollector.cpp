#ifndef METAL_SCRAPPER_SelfStatsCollector_CPP
#define METAL_SCRAPPER_SelfStatsCollector_CPP

#include "Collector.h"
#include "../Node/SelfStatsNode.cpp"

class SelfStatsCollector : public Collector {
public:
    SelfStatsCollector(Master *master) : Collector(master) {
        RE2::Options options;
        options.set_dot_nl(true);

        this->regex = new RE2(R"(.*VmRSS:\s+(\d+).*)", options);
        assert(this->regex->ok());

        this->procPath = master->procLocation + "/self/status";
        master->addCollector(this);
    }

    void collect() override {
        auto data = read_file(this->procPath);
        auto node = new SelfStatsNode();

        RE2::PartialMatch(data, *this->regex, &node->bytesUsed);
        node->bytesUsed *= 1024;

        this->collected.emplace_back(node);
    }

    std::string getName() override {
        return "collector_self_stats";
    }

    int getCollectPeriodMs() override {
        return getPeriodFromConfig();
    }

    std::vector<std::string> getMetrics() override {
        return {
                "metal_scrapper_current_rss_bytes"
        };
    }
};

#endif