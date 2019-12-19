#include "src/Collector/CpuCollector.cpp"
#include "src/Fabric.h"
#include "src/Clickhouse.h"
#include "src/util.h"
#include "test/test.h"
#include <fmt/printf.h>
#include <cassert>


int main() {
//    Fabric f;
//    CpuCollector cpuCollector;
//
//    f.addCollector(&cpuCollector);
//    f.work();

//    Clickhouse ch;
//
//    ch.connect();

//    function_time([]() {
//        char *p;
//        long converted = strtol("12345511t.1", &p, 10);
//        if (*p) {
//            return;
//        } else {
//            return;
//        }
//    });
//
//
//    function_time([]() {
//        is_float("12345511t.1");
//    });
//
//    function_time([]() {
//        is_float_fast("12345511t.1");
//    });

    assert(!is_float_fast("1,22"));
    assert(is_float_fast("1.22"));
    assert(is_float_fast("1.223321312"));
    assert(!is_float_fast("1,223321312"));
    assert(is_float_fast("1"));
    assert(is_float_fast("2"));
    assert(is_float_fast("0"));
    assert(!is_float_fast("test"));
    assert(!is_float_fast("test2"));
    assert(!is_float_fast("2test"));
    assert(!is_float_fast("t3est"));


    return 0;
}
