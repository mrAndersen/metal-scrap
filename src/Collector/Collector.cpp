#ifndef METAL_SCRAPPER_COLLECTOR_CPP
#define METAL_SCRAPPER_COLLECTOR_CPP

#include "Collector.h"

std::string Collector::readFile(const std::string &path) {
    std::ifstream t(path);
    std::string string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    return string;
}

std::vector<Node *> Collector::getCollectedData() {
    return this->collected;
}

Collector::Collector() {

}

void Collector::clear() {
    for (auto &v:this->collected) {
        delete (v);
    }

    collected.clear();
}

#endif