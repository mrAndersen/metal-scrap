#ifndef METAL_SCRAPPER_CPUNODE_CPP
#define METAL_SCRAPPER_CPUNODE_CPP

#include <string>
#include <vector>
#include <map>
#include "Node.h"

class CpuNode : public Node {
public:
    float load1 = 0;
    float load5 = 0;
    float load15 = 0;
    int processCount = 0;

    std::vector<std::pair<std::string, std::string>> prepare() override {
        std::vector<std::pair<std::string, std::string>> map;

        map.emplace_back(std::pair("cpu_load_1", std::to_string(this->load1)));
        map.emplace_back(std::pair("cpu_load_5", std::to_string(this->load5)));
        map.emplace_back(std::pair("cpu_load_15", std::to_string(this->load15)));
        map.emplace_back(std::pair("cpu_ps_count", std::to_string(this->processCount)));

        return map;
    }
};

#endif