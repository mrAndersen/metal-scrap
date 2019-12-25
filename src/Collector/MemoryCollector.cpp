#ifndef METAL_SCRAPPER_MemoryCollector_CPP
#define METAL_SCRAPPER_MemoryCollector_CPP

#include "Collector.h"
#include "../Node/MemoryNode.cpp"

class MemoryCollector : public Collector {
public:
    MemoryCollector(Master *master) : Collector(master) {
        RE2::Options options;
        options.set_dot_nl(true);

        this->regex = new RE2(R"(.*MemTotal:\s+(\d+).*MemAvailable:\s+(\d+))", options);
        assert(this->regex->ok());

        this->procPath = master->procLocation + "/meminfo";
        master->addCollector(this);
    }

    void collect() override {
        auto data = read_file(this->procPath);

        auto node = new MemoryNode();
        RE2::PartialMatch(data, *this->regex, &node->totalBytes, &node->freeBytes);

        this->collected.emplace_back(node);
    }

    std::string getName() override {
        return "collector_memory";
    }

    int getCollectPeriodMs() override {
        return getPeriodFromConfig();
    }

    std::vector<std::string> getMetrics() override {
        return {
                "memory_total_bytes",
                "memory_free_bytes"
        };
    }
};

#endif