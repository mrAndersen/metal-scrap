#ifndef METAL_SCRAPPER_COLLECTOR_CPP
#define METAL_SCRAPPER_COLLECTOR_CPP

#include "Collector.h"

const std::vector<Node *> &Collector::getCollectedData() {
    return this->collected;
}

void Collector::clear() {
    for (auto &v:this->collected) {
        delete (v);
    }

    collected.clear();
}

Collector::~Collector() {
    this->clear();
}

Collector::Collector(Master *master) {
    this->master = master;
}

#endif