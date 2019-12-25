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

    std::vector<std::map<std::string, std::string>> prepare() override {
        std::vector<std::map<std::string, std::string>> result;
        std::map<std::string, std::string> m;

        m["cpu_load_1"] = std::to_string(this->load1);
        m["cpu_load_5"] = std::to_string(this->load5);
        m["cpu_load_15"] = std::to_string(this->load15);
        m["cpu_ps_count"] = std::to_string(this->processCount);

        result.emplace_back(m);
        return result;
    }
};

#endif