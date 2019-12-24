metal-scrapper is a tool to collect machine metrics and flush it to clickhouse. To run docker contaier execute
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

After program will be launched - it will create 2 tables to store float and string metric values separated

Currently daemon has these metrics:

* ```cpu_load_1``` - load average 1 min
* ```cpu_load_5``` - load average 5 min
* ```cpu_load_15``` - load average 15 min
* ```cpu_ps_count``` - current process count
* ```hdd_total_bytes``` - total bytes at ```/``` filesystem
* ```hdd_free_bytes``` - free bytes at ```/``` filesystem
* ```memory_total_bytes``` - total bytes of memory
* ```memory_free_bytes``` - available bytes of memory
* ```metal_scrapper_current_rss_bytes``` - current memory usage for metal-scrapper