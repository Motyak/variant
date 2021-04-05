FROM alpine:3.13

ADD src src

RUN apk add --no-cache g++ && \
    apk add --no-cache make && \
    apk add --no-cache cmake && \
    mkdir -p usr/local/include && \
    cp -r src/cereal/include/cereal usr/local/include/cereal && \
    cd src/hiredis && \
    make && \
    make install && \
    cd ../redis-plus-plus && \
    mkdir build && \
    cd build && \
    cmake -DREDIS_PLUS_PLUS_CXX_STANDARD=17 -DREDIS_PLUS_PLUS_BUILD_TEST=OFF -DREDIS_PLUS_PLUS_BUILD_STATIC=OFF .. && \
    make && \
    make install && \
    export LD_LIBRARY_PATH=/usr/local/lib && \
    ldconfig /etc/ld.so.conf.d && \
    g++ -std=c++17 /src/main.cpp -o /evolueur -lredis++ -lhiredis -lstdc++fs -pthread && \
    cd / && \
    rm -rf src && \
    apk del g++ make cmake && \
    apk add --no-cache libstdc++

ENTRYPOINT ["./evolueur"]