#include "src/Collector/CpuCollector.cpp"
#include "src/Fabric.h"
#include <fmt/printf.h>


int main() {
    Fabric f;
    CpuCollector cpuCollector;

    f.addCollector(&cpuCollector);
    f.processCollectors();


//    fmt::printf("Size: %d", cpuCollector.getCollectedData().size());
    return 0;
}
