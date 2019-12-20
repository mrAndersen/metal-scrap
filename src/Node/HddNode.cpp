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

    std::vector<std::pair<std::string, std::string>> prepare() override {
        std::vector<std::pair<std::string, std::string>> map;

        map.emplace_back(std::pair("hdd_total_bytes", std::to_string(this->totalBytes)));
        map.emplace_back(std::pair("hdd_free_bytes", std::to_string(this->freeBytes)));

        return map;
    }
};

#endif