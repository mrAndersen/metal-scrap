cmake_minimum_required(VERSION 3.10)
project(metal_scrapper)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")
SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -pthread")

set(CMAKE_CXX_STANDARD 17)
set(
        SOURCES
        main.cpp

        vendor/fmt/src/format.cc
        src/Collector/Collector.h src/Collector/CpuCollector.cpp src/Collector/Collector.cpp src/Node/CpuNode.cpp src/Node/Node.h src/Fabric.cpp src/Fabric.h)

add_executable(metal_scrapper ${SOURCES})
target_link_libraries(metal_scrapper ${CURL_LIBRARIES})
target_include_directories(metal_scrapper PUBLIC ${CURL_INCLUDE_DIR} vendor/fmt/include)