#include <thread>
#include <fmt/printf.h>
#include <mutex>
#include "Fabric.h"

int Fabric::addCollector(Collector *collector) {
    this->collectors.emplace_back(collector);
    return collectors.size();
}

void Fabric::work() {
    std::vector<Node *> buffer;
    std::mutex flushMutex;


    std::thread monitorThread([&]() {
        int monitorPeriodMs = 2000;

        while (true) {
            for (auto &collector:collectors) {
                fmt::printf("%s, entries = %d\n", collector->getName(), collector->getCollectedData().size());
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(monitorPeriodMs));
        }
    });

    monitorThread.detach();

    std::thread flushThread([&]() {
        int flushPeriodMs = 2000;
        std::vector<std::pair<std::string, std::string>> buffer;

        while (true) {
            for (auto &collector:collectors) {
                if (collector->getCollectedData().empty()) {
                    continue;
                }

                flushMutex.lock();

                auto data = collector->getCollectedData();

                for (auto &node:data) {
                    auto local = node->prepare();
                    buffer.insert(buffer.end(), local.begin(), local.end());
                }

                collector->clear();
                flushMutex.unlock();
            }

            fmt::printf("buffer size = %d\n", buffer.size());
            std::this_thread::sleep_for(std::chrono::milliseconds(flushPeriodMs));
        }
    });

    flushThread.detach();

    for (auto &collector:collectors) {
        std::thread t([collector]() {
            while (true) {
                collector->collect();

                std::this_thread::sleep_for(std::chrono::milliseconds(collector->getCollectPeriodMs()));
            }
        });

        t.detach();
    }

    while (true) {

    }


    exit(1);

}
