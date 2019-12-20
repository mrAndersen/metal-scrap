#ifndef METAL_SCRAPPER_SelfStatsNode_CPP
#define METAL_SCRAPPER_SelfStatsNode_CPP

#include <string>
#include <vector>
#include <map>
#include "Node.h"

class SelfStatsNode : public Node {
public:
    long bytesUsed = 0;

    std::vector<std::pair<std::string, std::string>> prepare() override {
        std::vector<std::pair<std::string, std::string>> map;

        map.emplace_back(std::pair("metal_scrapper_current_rss_bytes", std::to_string(this->bytesUsed)));

        return map;
    }
};

#endif