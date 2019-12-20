#include "Clickhouse.h"


bool Clickhouse::writeStringBlock(const std::vector<std::pair<std::string, std::string>> &buffer) {
    auto stringBlock = std::make_shared<clickhouse::Block>();

    auto chTitle = std::make_shared<clickhouse::ColumnString>();
    auto chStringValue = std::make_shared<clickhouse::ColumnString>();
    auto chHostname = std::make_shared<clickhouse::ColumnString>();

    for (auto &stringPair:buffer) {
        chTitle->Append(stringPair.first);
        chStringValue->Append(stringPair.second);
        chHostname->Append(this->master->hostname);
    }

    stringBlock->AppendColumn("metric_title", chTitle);
    stringBlock->AppendColumn("metric_value", chStringValue);
    stringBlock->AppendColumn("hostname", chHostname);

    connectionMutex.lock();
    client->Insert("metal_string", *stringBlock);
    connectionMutex.unlock();

    return true;
}

bool Clickhouse::writeFloatBlock(const std::vector<std::pair<std::string, float>> &buffer) {
    auto floatBlock = std::make_shared<clickhouse::Block>();

    auto chTitle = std::make_shared<clickhouse::ColumnString>();
    auto chFloatValue = std::make_shared<clickhouse::ColumnFloat32>();
    auto chHostname = std::make_shared<clickhouse::ColumnString>();

    for (auto &stringPair:buffer) {
        chTitle->Append(stringPair.first);
        chFloatValue->Append(stringPair.second);
        chHostname->Append(this->master->hostname);
    }

    floatBlock->AppendColumn("metric_title", chTitle);
    floatBlock->AppendColumn("metric_value", chFloatValue);
    floatBlock->AppendColumn("hostname", chHostname);

    connectionMutex.lock();
    client->Insert("metal_float", *floatBlock);
    connectionMutex.unlock();

    return true;
}

bool Clickhouse::write(std::vector<std::pair<std::string, std::string>> *buffer) {
    std::vector<std::pair<std::string, float>> floatBuffer;
    std::vector<std::pair<std::string, std::string>> stringBuffer;

    START_TIME;

    for (auto &pair:*buffer) {
        auto title = pair.first;
        auto value = pair.second;

        if (is_float(value)) {
            floatBuffer.emplace_back(std::pair<std::string, float>(title, std::stof(value)));
        } else {
            stringBuffer.emplace_back(pair);
        }
    }

    std::thread f([&]() {
        this->writeFloatBlock(floatBuffer);
    });

    std::thread s([&]() {
        this->writeStringBlock(stringBuffer);
    });

    f.join();
    s.join();

    if (this->master->verbosity >= 1) {
        message_ok("%-40s %d ms", "buffer flushed in", END_TIME_MS);
        message_ok("%-40s %d", "rows inserted", buffer->size());
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
