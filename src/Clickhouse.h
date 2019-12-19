#ifndef METAL_SCRAPPER_CLICKHOUSE_H
#define METAL_SCRAPPER_CLICKHOUSE_H

#include <clickhouse/client.h>
#include <fmt/printf.h>

class Clickhouse {
protected:
    clickhouse::Client *client;

    std::string host;
    int port;
    std::string user;
    std::string password;

public:
    Clickhouse(
            const std::string &host = "localhost",
            const int &port = 9000,
            const std::string &user = "default",
            const std::string &password = ""
    );

    bool connect();

    bool ping();

    void tryCreateTables();

    bool write(const std::vector<std::pair<std::string, std::string>> &buffer);

};


#endif //METAL_SCRAPPER_CLICKHOUSE_H
