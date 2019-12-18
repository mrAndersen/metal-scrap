#include "Clickhouse.h"


bool Clickhouse::ping() {








}

bool Clickhouse::connect() {

    clickhouse::ClientOptions options;
    options.SetHost("admile.ru");
    options.SetPort(9000);
    options.SetUser("default");

    clickhouse::Client t(options);
    t.Select("select 1");

}
