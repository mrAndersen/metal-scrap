
#include "Master.h"


Master::Master(const YAML::Node &config) : config(config) {
    std::string host = "localhost";
    int port = 9000;
    std::string user = "default";
    std::string password;

    if (config["settings"]["proc_location"].IsDefined()) {
        this->procLocation = config["settings"]["proc_location"].as<std::string>();
    }

    if (config["settings"]["hostname"].IsDefined()) {
        this->hostname = config["settings"]["hostname"].as<std::string>();
    }

    if (config["settings"]["verbosity"].IsDefined()) {
        this->verbosity = config["settings"]["verbosity"].as<int>();
    }

    if (config["settings"]["buffer"].IsDefined() && config["settings"]["buffer"]["flush_period"].IsDefined()) {
        this->flushPeriodMs = config["settings"]["buffer"]["flush_period"].as<int>();
    }

    if (config["settings"]["clickhouse"]["host"].IsDefined()) {
        host = config["settings"]["clickhouse"]["host"].as<std::string>();
    }

    if (config["settings"]["clickhouse"]["port"].IsDefined()) {
        port = config["settings"]["clickhouse"]["port"].as<int>();
    }

    if (config["settings"]["clickhouse"]["user"].IsDefined()) {
        user = config["settings"]["clickhouse"]["user"].as<std::string>();
    }

    if (config["settings"]["clickhouse"]["password"].IsDefined()) {
        password = config["settings"]["clickhouse"]["password"].as<std::string>();
    }

    if (!file_exists(this->procLocation + "/uptime")) {
        message_error("Proc location %s is invalid or does not exist, correct your config", this->procLocation.c_str());
        exit(1);
    }

    this->clickhouse = new Clickhouse(host, port);

    this->clickhouse->setMaster(this);
    this->clickhouse->setUser(user);
    this->clickhouse->setPassword(password);

    message_ok(
            "Master created, hostname = %s, proc_location = %s, flush period = %d",
            this->hostname.c_str(),
            this->procLocation.c_str(),
            this->flushPeriodMs
    );
}


int Master::addCollector(Collector *collector) {
    this->collectors.emplace_back(collector);

    message_ok("Collector %s added, period = %dms", collector->getName().c_str(), collector->getCollectPeriodMs());
    return collectors.size();
}


void Master::work() {
    std::vector<Node *> buffer;
    std::mutex collectorMutex;
    int monitorPeriodMs = 2000;

    this->printMetrics();
    this->clickhouse->connect();

    std::thread flushThread([&]() {
        auto buffer = new std::vector<Node *>();
        int retries = 100;

        while (true) {
            for (auto &collector:collectors) {
                if (collector->getCollectedData().empty()) {
                    continue;
                }

                collectorMutex.lock();

                auto collected = collector->getCollectedData();
                buffer->insert(buffer->end(), collected.begin(), collected.end());

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

            for (auto &collector:collectors) {
                collector->clear();
            }

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

void Master::printMetrics() {
    if (verbosity > 0) {
        message_ok("Collected metric:");

        for (auto &collector:collectors) {
            for (auto &metric:collector->getMetrics()) {
                message_ok(metric.c_str());
            }
        }
    }
}






