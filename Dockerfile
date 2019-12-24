FROM debian:buster

RUN apt-get update && apt-get install -y \
    libc-dev git gcc g++ cmake gdb

WORKDIR /metal

RUN rm -rf /metal/bin \
    && mkdir /metal/bin

COPY ./src /metal/src
COPY ./CMakeLists.txt /metal/CMakeLists.txt
COPY ./vendor /metal/vendor
COPY ./example.yml /metal/example.yml
COPY ./main.cpp /metal/main.cpp

RUN cmake -B /metal/bin -DCMAKE_BUILD_TYPE=Release . \
    && cd /metal/bin \
    && make -j 8 -o2

CMD /metal/bin/metal_scrapper