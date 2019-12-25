#include "Clickhouse.h"

bool Clickhouse::write(std::vector<Node *> *buffer) {
    START_TIME;

    auto sBlock = std::make_shared<clickhouse::Block>();
    auto fBlock = std::make_shared<clickhouse::Block>();

    auto sTitle = std::make_shared<clickhouse::ColumnString>();
    auto sValue = std::make_shared<clickhouse::ColumnString>();
    auto sHostname = std::make_shared<clickhouse::ColumnString>();
    auto sDateTime = std::make_shared<clickhouse::ColumnDateTime>();

    auto fTitle = std::make_shared<clickhouse::ColumnString>();
    auto fValue = std::make_shared<clickhouse::ColumnFloat32>();
    auto fHostname = std::make_shared<clickhouse::ColumnString>();
    auto fDateTime = std::make_shared<clickhouse::ColumnDateTime>();

    int i = 0;

    for (auto &node:*buffer) {
        auto metrics = node->prepare();

        for (auto &metric:metrics) {
            for (auto &v:metric) {
                auto title = v.first;
                auto value = v.second;

                if (is_float(value)) {
                    fTitle->Append(title);
                    fValue->Append(std::stof(value));
                    fHostname->Append(master->hostname);
                    fDateTime->Append(node->time);
                } else {
                    fTitle->Append(title);
                    sValue->Append(value);
                    fHostname->Append(master->hostname);
                    fDateTime->Append(node->time);
                }

                i++;
            }
        }
    }

    fBlock->AppendColumn("metric_title", fTitle);
    fBlock->AppendColumn("metric_value", fValue);
    fBlock->AppendColumn("hostname", fHostname);
    fBlock->AppendColumn("datetime", fDateTime);

    sBlock->AppendColumn("metric_title", sTitle);
    sBlock->AppendColumn("metric_value", sValue);
    sBlock->AppendColumn("hostname", sHostname);
    sBlock->AppendColumn("datetime", sDateTime);

    std::thread f([&]() {
        connectionMutex.lock();
        client->Insert("metal_float", *fBlock);
        connectionMutex.unlock();
    });

    std::thread s([&]() {
        connectionMutex.lock();
        client->Insert("metal_string", *sBlock);
        connectionMutex.unlock();
    });

    f.join();
    s.join();

    if (this->master->verbosity >= 1) {
        message_ok("%-40s %d ms", "buffer flushed in", END_TIME_MS);
        message_ok("%-40s %d", "rows inserted", i);
    }

    return true;
}

void Clickhouse::tryCreateTables() {
    std::string floatTableSql = R"(
create table if not exists metal_float
(
    metric_title String,
    metric_value Float32,
    date         Date default toDate(datetime),
    datetime     DateTime default now(),
    hostname     String
) engine = MergeTree(date, (metric_title, metric_value, hostname), 4096);
)";

    std::string stringTableSql = R"(
create table if not exists metal_string
(
    metric_title String,
    metric_value String,
    date         Date default toDate(datetime),
    datetime     DateTime default now(),
    hostname     String
) engine = MergeTree(date, (metric_title, metric_value, hostname), 4096);
)";

    this->client->Execute(clickhouse::Query(floatTableSql));
    this->client->Execute(clickhouse::Query(stringTableSql));
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

    this->tryCreateTables();

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

Clickhouse::Clickhouse(const std::string &host, const int &port) {
    this->host = host;
    this->port = port;
}

void Clickhouse::setMaster(Master *master) {
    Clickhouse::master = master;
}
