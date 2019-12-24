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

class Collector;

class Master {
protected:
    std::vector<Collector *> collectors;
    Clickhouse *clickhouse = nullptr;

public:
    YAML::Node config;

    int flushPeriodMs = 2000;

    std::string hostname = "some_host";

    std::string procLocation = "/proc";

    int verbosity = 0;

    int addCollector(Collector *collector);

    void work();

    void printMetrics();

    Master(const YAML::Node &config);
};

#endif //METAL_SCRAPPER_MASTER_H
