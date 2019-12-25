FROM alpine:3.11.2

# setup APK
RUN apk update \
    && apk upgrade \
    && apk add ca-certificates \
    && apk add libc-dev gcc make \
    && apk add bash \
    && apk add g++ \
    && apk add git \
    && apk add cmake \
    && apk add gdb

WORKDIR /metal

RUN rm -rf /metal/bin \
    && mkdir /metal/bin

COPY ./src /metal/src
COPY ./CMakeLists.txt /metal/CMakeLists.txt
COPY ./vendor /metal/vendor
COPY ./example.yml /metal/example.yml
COPY ./main.cpp /metal/main.cpp

RUN cmake -B /metal/bin -DCMAKE_BUILD_TYPE=Debug . \
    && cd /metal/bin \
    && make -j 8 -o2

#CMD /metal/bin/metal_scrapper
CMD tail -f /dev/null