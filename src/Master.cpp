#include "Master.h"


Master::Master(const YAML::Node &config) : config(config) {
    this->hostname = config["settings"]["hostname"].as<std::string>();
    this->verbosity = config["settings"]["verbosity"].as<int>();

    auto host = config["settings"]["clickhouse"]["host"].as<std::string>();
    auto port = config["settings"]["clickhouse"]["port"].as<int>();
    auto user = config["settings"]["clickhouse"]["user"].as<std::string>();
    auto password = config["settings"]["clickhouse"]["password"].as<std::string>();

    this->clickhouse = new Clickhouse(host, port);

    this->clickhouse->setMaster(this);
    this->clickhouse->setUser(user);
    this->clickhouse->setPassword(password);

    message_ok("Master created, hostname = %s", this->hostname.c_str());
}

int Master::addCollector(Collector *collector) {
    this->collectors.emplace_back(collector);

    message_ok("Collector %s added", collector->getName().c_str());

    return collectors.size();
}


void Master::work() {
    std::vector<Node *> buffer;
    std::mutex collectorMutex;

    int monitorPeriodMs = 2000;

    this->clickhouse->connect();

    std::thread flushThread([&]() {
        int flushPeriodMs = 1200;
        auto buffer = new std::vector<std::pair<std::string, std::string>>();
        int retries = 100;

        while (true) {
            for (auto &collector:collectors) {
                if (collector->getCollectedData().empty()) {
                    continue;
                }

                collectorMutex.lock();
                auto data = collector->getCollectedData();
                for (auto &node:data) {
                    auto local = node->prepare();
                    buffer->insert(buffer->end(), local.begin(), local.end());
                }

                collector->clear();
                collectorMutex.unlock();
            }

            int i = 0;
            while (!this->clickhouse->isConnected() && i <= retries) {
                this->clickhouse->connect();

                i++;
                std::this_thread::sleep_for(std::chrono::seconds(2));
            }

            if (i >= retries) {
                message_error("Could not connect to clickhouse");
                exit(1);
            }

            this->clickhouse->write(buffer);
            buffer->clear();


            std::this_thread::sleep_for(std::chrono::milliseconds(flushPeriodMs));
        }
    });

    flushThread.detach();

    for (auto &collector:collectors) {
        std::thread t([&, collector]() {
            while (true) {
                START_TIME;

                collector->collect();

                if (this->verbosity >= 2) {
                    message_ok("%-40s %.4f ms", collector->getName().c_str(), (double) END_TIME_NS / 1000 / 1000);
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(collector->getCollectPeriodMs()));
            }
        });

        t.detach();
    }

    while (true) {

        //do some stuff

        std::this_thread::sleep_for(std::chrono::milliseconds(monitorPeriodMs));
    }

}


