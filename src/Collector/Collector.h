#ifndef METAL_SCRAPPER_COLLECTOR_H
#define METAL_SCRAPPER_COLLECTOR_H

#include <string>
#include <regex>
#include <fmt/format.h>
#include <fstream>
#include "../Node/Node.h"
#include "../util.h"
#include "../Master.h"

class Master;

class Collector {
protected:
    std::string procPath;
    std::regex regex;
    std::vector<Node *> collected;
    Master *master;
public:
    Collector(Master *master);

    virtual void collect() = 0;

    void clear();

    virtual const std::vector<Node *> &getCollectedData();

    virtual std::vector<std::string> getMetrics() = 0;

    virtual int getCollectPeriodMs() = 0;

    virtual std::string getName() = 0;

    virtual ~Collector();
};

#endif //METAL_SCRAPPER_COLLECTOR_H
