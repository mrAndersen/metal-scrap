#ifndef METAL_SCRAPPER_CpuCollector_CPP
#define METAL_SCRAPPER_CpuCollector_CPP

#include "Collector.h"
#include "../Node/CpuNode.cpp"

class CpuCollector : public Collector {
public:
    CpuCollector(Master *master) : Collector(master) {
        this->regex = new RE2(R"((\d+.\d+)\s(\d+.\d+)\s(\d+.\d+)\s\d+\/(\d+)\s\d+)");
        assert(this->regex->ok());

        this->procPath = master->procLocation + "/loadavg";

        master->addCollector(this);
    }

    void collect() override {
        auto data = read_file(this->procPath);
        auto node = new CpuNode();

        RE2::PartialMatch(data, *this->regex, &node->load1, &node->load5, &node->load15, &node->processCount);

        this->collected.emplace_back(node);
    }

    std::string getName() override {
        return "collector_cpu";
    }

    int getCollectPeriodMs() override {
        return getPeriodFromConfig();
    }

    std::vector<std::string> getMetrics() override {
        return {
                "cpu_load_1",
                "cpu_load_5",
                "cpu_load_15",
                "cpu_ps_count",
        };
    }
};

#endif