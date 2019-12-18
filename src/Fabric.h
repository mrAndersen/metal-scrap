#ifndef METAL_SCRAPPER_FABRIC_H
#define METAL_SCRAPPER_FABRIC_H

#include <vector>
#include "Collector/Collector.h"

class Fabric {
protected:
    std::vector<Collector *> collectors;

public:
    int addCollector(Collector * collector);

    void work();
};

#endif //METAL_SCRAPPER_FABRIC_H
