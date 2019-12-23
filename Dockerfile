FROM alpine:edge

# setup APK
RUN apk update \
    && apk upgrade \
    && apk add ca-certificates \
    && apk add libc-dev gcc make \
    && apk add bash \
    && apk add g++ \
    && apk add git \
    && apk add cmake

WORKDIR /metal

RUN rm -rf /metal/bin \
    && mkdir /metal/bin

COPY ./src /metal/src
COPY ./CMakeLists.txt /metal/CMakeLists.txt
COPY ./vendor /metal/vendor
COPY ./example.yml /metal/example.yml
COPY ./main.cpp /metal/main.cpp

RUN cmake -B /metal/bin . \
    && cd /metal/bin \
    && make -j 8

CMD /metal/bin/metal_scrapper