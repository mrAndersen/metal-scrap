#ifndef METAL_SCRAPPER_COLLECTOR_H
#define METAL_SCRAPPER_COLLECTOR_H

#include <string>
#include <regex>
#include <fmt/format.h>
#include <fstream>
#include "../Node/Node.h"
#include "../util.h"


class Collector {
protected:
    std::string procPath;
    std::regex regex;
    std::vector<Node *> collected;
public:
    Collector();

    virtual void collect() = 0;

    void clear();

    virtual const std::vector<Node *> &getCollectedData();

    virtual int getCollectPeriodMs() = 0;

    virtual std::string getName() = 0;

    virtual ~Collector();
};

#endif //METAL_SCRAPPER_COLLECTOR_H
