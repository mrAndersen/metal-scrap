#ifndef METAL_SCRAPPER_MASTER_H
#define METAL_SCRAPPER_MASTER_H

#include <vector>
#include "Collector/Collector.h"
#include "Clickhouse.h"
#include <thread>
#include <fmt/printf.h>
#include <mutex>
#include "util.h"
#include <yaml-cpp/yaml.h>

class Clickhouse;

class Master {
protected:
    std::vector<Collector *> collectors;
    Clickhouse *clickhouse = nullptr;
    YAML::Node config;
public:
    int flushPeriodMs = 2000;

    std::string hostname = "some_host";

    int verbosity = 0;

    int addCollector(Collector *collector);

    void work();

    void printMetrics();

    Master(const YAML::Node &config);
};

#endif //METAL_SCRAPPER_MASTER_H