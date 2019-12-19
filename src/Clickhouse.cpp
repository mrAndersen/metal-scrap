#include <iostream>
#include "Clickhouse.h"
#include "util.h"


bool Clickhouse::write(const std::vector<std::pair<std::string, std::string>> &buffer) {

    for (auto &pair:buffer) {
        auto title = pair.first;
        auto value = pair.second;




//        auto id = std::make_shared<ColumnUInt64>();
//        id->Append(1);
//        id->Append(7);
//
//        auto name = std::make_shared<ColumnString>();
//        name->Append("one");
//        name->Append("seven");
//
//        block.AppendColumn("id"  , id);
//        block.AppendColumn("name", name);
//
//        client.Insert("test.numbers", block);


    }


    return false;
}

void Clickhouse::tryCreateTables() {
    std::string floatTableSql = R"(
create table if not exists metal_float
(
    metric_title FixedString(100),
    metric_value Float64,
    date         Date default toDate(datetime),
    datetime     DateTime
) engine = MergeTree(date, (metric_title, metric_value), 1024);
)";

    std::string stringTableSql = R"(
create table if not exists metal_string
(
    metric_title FixedString(100),
    metric_value String,
    date         Date default toDate(datetime),
    datetime     DateTime
) engine = MergeTree(date, (metric_title, metric_value), 1024);
)";

    this->client->Execute(clickhouse::Query(floatTableSql));
    this->client->Execute(clickhouse::Query(stringTableSql));
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

    message_ok("Connected to clickhouse at %s:%s", options.host.c_str(), std::to_string(options.port).c_str());
    return true;
}

Clickhouse::Clickhouse(
        const std::string &host,
        const int &port,
        const std::string &user,
        const std::string &password
) : host(host), port(port), user(user), password(password) {}


