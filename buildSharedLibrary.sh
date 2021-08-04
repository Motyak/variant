#!/bin/sh
g++ -fPIC -c src/_RedisPlusPlusBny.cpp -o _RedisPlusPlusBny.o --std=c++17
g++ _RedisPlusPlusBny.o -shared -o libevo.so -L /usr/local/lib -lhiredis -lredis++
g++ src/main.cpp -o evolueur -lstdc++fs -pthread --std=c++17 ./libevo.so
