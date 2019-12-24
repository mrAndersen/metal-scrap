metal-scrapper is a tool to collect machine metrics and flush it to clickhouse. To run docker container execute
```bash
docker run -d --name metal \
    -v /proc:/hostproc \
    -v /docker-data/metal_config.yml:/metal-config.yml \
    -e CONFIG=/metal-config.yml \
mrandersen7/metal:1.1
```

This will volume your configuration file and use it to run the daemon. Example config is located in
example.yml

* ```proc_locaton``` - where linux proc directory located (volumed)
* ```hostname``` - hostname of machine which will be inserted into CH
* ```verbosity``` - verbosity of log output (2 - all, 1 - partly, 0 - none)
* ```buffer.flush_period``` - flush period in ms
* ```clickhouse``` - section for clickhouse connection.

After program will be launched - it will create 2 tables to store float and string metric values separated in your clickhouse connection in default database, after that u can aggregate them via grafana, or any other tools
