#ifndef METAL_SCRAPPER_NODE_H
#define METAL_SCRAPPER_NODE_H

#include <ctime>

class Node {
public:
    std::time_t time;

    virtual std::vector<std::map<std::string, std::string>> prepare() = 0;

    Node() {
        time = std::time(nullptr);
    }
};

#endif //METAL_SCRAPPER_NODE_H