#ifndef METAL_SCRAPPER_CLICKHOUSE_H
#define METAL_SCRAPPER_CLICKHOUSE_H

#include <clickhouse/client.h>
#include <fmt/printf.h>
#include <mutex>
#include <iostream>
#include <thread>
#include "util.h"
#include "Master.h"

class Master;

class Clickhouse {
protected:
    Master *master;

    clickhouse::Client *client;
    std::mutex connectionMutex;

    std::string host = "localhost";
    int port = 9000;
    std::string user = "default";
    std::string password = "";

public:
    Clickhouse(const std::string &host = "localhost", const int &port = 9000);

    bool connect();

    bool isConnected();

    void tryCreateTables();

    bool write(std::vector<Node *> *buffer);

    void setHost(const std::string &host);

    void setPort(int port);

    void setUser(const std::string &user);

    void setPassword(const std::string &password);

    void setMaster(Master *master);
};


#endif //METAL_SCRAPPER_CLICKHOUSE_H
