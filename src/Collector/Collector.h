#ifndef METAL_SCRAPPER_COLLECTOR_H
#define METAL_SCRAPPER_COLLECTOR_H

#include <string>
#include <regex>
#include <fmt/format.h>
#include <fstream>
#include "../Node/Node.h"


class Collector {
protected:
    std::string procPath;
    std::regex regex;
    std::vector<Node *> collected;
public:
    Collector();

    virtual std::string readFile(const std::string &path);

    virtual void collect() = 0;

    virtual void clear();

    virtual const std::vector<Node *> &getCollectedData();

    virtual int getCollectPeriodMs() = 0;

    virtual std::string getName() = 0;
};

#endif //METAL_SCRAPPER_COLLECTOR_H
