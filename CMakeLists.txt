cmake_minimum_required(VERSION 3.10)
project(metal_scrapper)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

add_subdirectory(vendor/clickhouse)
add_subdirectory(vendor/yaml-cpp)

set(CMAKE_CXX_STANDARD 17)
set(
        SOURCES
        main.cpp

        vendor/fmt/src/format.cc
        src/Node/Node.h

        src/Collector/Collector.h
        src/Collector/Collector.cpp

        src/Collector/CpuCollector.cpp
        src/Collector/SelfStatsCollector.cpp

        src/Node/CpuNode.cpp
        src/Node/SelfStatsNode.cpp

        src/Master.cpp
        src/Master.h

        src/Clickhouse.cpp
        src/Clickhouse.h
        src/util.h
        src/util.cpp

        test/test.h
        test/test.cpp

        src/Collector/HddCollector.cpp
        src/Node/HddNode.cpp

        src/Collector/MemoryCollector.cpp
        src/Node/MemoryNode.cpp
)


add_executable(metal_scrapper ${SOURCES})


target_link_libraries(
        metal_scrapper
        clickhouse-cpp-lib
        yaml-cpp
)

target_include_directories(
        metal_scrapper PUBLIC

        vendor/yaml-cpp/include
        vendor/clickhouse
        vendor/fmt/include
)