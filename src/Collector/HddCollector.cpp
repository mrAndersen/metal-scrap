#ifndef METAL_SCRAPPER_HDDCOLLECTOR_CPP
#define METAL_SCRAPPER_HDDCOLLECTOR_CPP

#include "Collector.h"
#include <iostream>
#include <sys/statvfs.h>
#include "../Node/HddNode.cpp"

class HddCollector : public Collector {
public:
    HddCollector(Master *master) : Collector(master) {
        master->addCollector(this);
    }

    void collect() override {
        struct statvfs stats{};
        statvfs("/", &stats);

        auto node = new HddNode();
        node->totalBytes = stats.f_blocks * stats.f_bsize;
        node->freeBytes = stats.f_bfree * stats.f_bsize;

        this->collected.emplace_back(node);
    }

    std::string getName() override {
        return "collector_hdd";
    }

    int getCollectPeriodMs() override {
        return 1000;
    }
};

#endif