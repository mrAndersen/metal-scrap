#include "Clickhouse.h"

bool Clickhouse::write(std::vector<Node *> *buffer) {
    START_TIME;

    auto block = std::make_shared<clickhouse::Block>();

    auto titleCol = std::make_shared<clickhouse::ColumnString>();
    auto sValueCol = std::make_shared<clickhouse::ColumnString>();
    auto fValueCol = std::make_shared<clickhouse::ColumnFloat64>();
    auto hostnameCol = std::make_shared<clickhouse::ColumnString>();
    auto dateTimeCol = std::make_shared<clickhouse::ColumnDateTime>();

    int i = 0;

    for (auto &node: *buffer) {
        auto metrics = node->prepare();

        for (auto &metric: metrics) {
            for (auto &v: metric) {
                auto title = v.first;
                auto value = v.second;

                titleCol->Append(title);
                hostnameCol->Append(master->hostname);
                dateTimeCol->Append(node->time);

                if (is_float(value)) {
                    fValueCol->Append(std::stof(value));
                } else {
                    sValueCol->Append(value);
                }

                i++;
            }
        }
    }

    block->AppendColumn("metric", titleCol);
    block->AppendColumn("stringValue", sValueCol);
    block->AppendColumn("floatValue", fValueCol);
    block->AppendColumn("time", dateTimeCol);

    std::thread f([&]() {
        connectionMutex.lock();
        client->Insert(table, *block);
        connectionMutex.unlock();
    });

    f.join();

    if (this->master->verbosity >= 1) {
        message_ok("%-40s %d ms", "buffer flushed in", END_TIME_MS);
        message_ok("%-40s %d", "rows inserted", i);
    }

    return true;
}

bool Clickhouse::isConnected() {
    try {
        client->Ping();
        return true;
    } catch (const std::runtime_error &e) {
        return false;
    }
}

bool Clickhouse::connect() {
    clickhouse::ClientOptions options;
    options.host = host;
    options.port = port;
    options.password = password;
    options.user = user;

    client = new clickhouse::Client(options);
    client->Ping();

    message_ok("Connected to clickhouse at %s:%s\n", options.host.c_str(), std::to_string(options.port).c_str());
    return true;
}

void Clickhouse::setHost(const std::string &host) {
    Clickhouse::host = host;
}

void Clickhouse::setPort(int port) {
    Clickhouse::port = port;
}

void Clickhouse::setUser(const std::string &user) {
    Clickhouse::user = user;
}

void Clickhouse::setPassword(const std::string &password) {
    Clickhouse::password = password;
}

Clickhouse::Clickhouse(
        const std::string &host,
        const int &port,
        const std::string &password,
        const std::string &table
) {
    this->host = host;
    this->port = port;
    this->password = password;
    this->table = table;
}

void Clickhouse::setMaster(Master *master) {
    Clickhouse::master = master;
}
