#ifndef METAL_SCRAPPER_NODE_H
#define METAL_SCRAPPER_NODE_H

class Node {
public:
    virtual std::vector<std::pair<std::string, std::string>> prepare() = 0;
};

#endif //METAL_SCRAPPER_NODE_H