#ifndef METAL_SCRAPPER_HddNode_CPP
#define METAL_SCRAPPER_HddNode_CPP

#include <string>
#include <vector>
#include <map>
#include "Node.h"

class HddNode : public Node {
public:
    unsigned long totalBytes = 0;
    unsigned long freeBytes = 0;

    std::vector<std::map<std::string, std::string>> prepare() override {
        std::vector<std::map<std::string, std::string>> result;
        std::map<std::string, std::string> m;

        m["hdd_total_bytes"] = std::to_string(this->totalBytes);
        m["hdd_free_bytes"] = std::to_string(this->freeBytes);

        result.emplace_back(m);
        return result;
    }
};

#endif