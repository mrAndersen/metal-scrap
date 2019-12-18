#include "src/Collector/CpuCollector.cpp"
#include "src/Fabric.h"
#include "src/Clickhouse.h"
#include <fmt/printf.h>


int main() {
//    Fabric f;
//    CpuCollector cpuCollector;
//
//    f.addCollector(&cpuCollector);
//    f.work();

    Clickhouse ch;

    ch.connect();


    return 0;
}
