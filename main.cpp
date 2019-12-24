#include "src/Master.h"
#include "src/Clickhouse.h"
#include "src/util.h"
//#include "test/test.h"
#include <fmt/printf.h>
#include "src/Collector/CpuCollector.cpp"
#include "src/Collector/SelfStatsCollector.cpp"
#include "src/Collector/HddCollector.cpp"
#include "src/Collector/MemoryCollector.cpp"

int main() {
    YAML::Node config;
    std::vector<std::string> settingsFileLocations;

    auto envConfig = getenv("CONFIG");

    if (envConfig) {
        settingsFileLocations.emplace_back(envConfig);
    } else {
        settingsFileLocations = {
                "config.yml",
                "../config.yml",
                "example.yml",
                "../example.yml"
        };
    }

    START_TIME;

    for (auto &path:settingsFileLocations) {
        if (file_exists(path)) {
            config = YAML::LoadFile(path);
            message_ok("Loaded configuration file = %s in %d ms", path.c_str(), END_TIME_MS);
            break;
        }
    }

    if (config.size() == 0) {
        message_error("Could not load any configuration file, check your paths");
        exit(1);
    }

    Master f(config);

    CpuCollector cpuCollector(&f);
    SelfStatsCollector selfStatsCollector(&f);
    HddCollector hddCollector(&f);
    MemoryCollector memoryCollector(&f);

    f.work();
    return 0;
}
