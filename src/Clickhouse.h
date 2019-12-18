#ifndef METAL_SCRAPPER_CLICKHOUSE_H
#define METAL_SCRAPPER_CLICKHOUSE_H

#include <clickhouse/client.h>

class Clickhouse {
protected:
    clickhouse::Client *client;

public:
    bool connect();

    bool ping();

};


#endif //METAL_SCRAPPER_CLICKHOUSE_H
