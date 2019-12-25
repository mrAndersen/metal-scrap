#ifndef METAL_SCRAPPER_SelfStatsNode_CPP
#define METAL_SCRAPPER_SelfStatsNode_CPP

#include <string>
#include <vector>
#include <map>
#include "Node.h"

class SelfStatsNode : public Node {
public:
    long bytesUsed = 0;

    std::vector<std::map<std::string, std::string>> prepare() override {
        std::vector<std::map<std::string, std::string>> result;
        std::map<std::string, std::string> m;

        m["metal_scrapper_current_rss_bytes"] = std::to_string(this->bytesUsed);

        result.emplace_back(m);
        return result;
    }
};

#endif