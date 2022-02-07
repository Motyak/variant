CC=g++
CFLAGS=-fPIC -I include --std=c++17
LFLAGS=-L /usr/local/lib -lhiredis -lredis++ -lstdc++fs -pthread
LIB=lib/libevo.so
OBJ=obj/env.o obj/Redis.o obj/Evolueur.o
DIRS=obj lib

all: $(LIB)

$(LIB): $(OBJ)
	$(CC) -shared -o $@ $^ $(LFLAGS)

obj/env.o: src/env.cpp include/env.h
	$(CC) -o $@ -c $< $(CFLAGS)

obj/Redis.o: src/Redis.cpp include/Redis.h include/Evoluable.hpp include/env.h
	$(CC) -o $@ -c $< $(CFLAGS)

obj/Evolueur.o: src/Evolueur.cpp include/Evolueur.h include/Redis.h include/Evoluable.hpp include/env.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf obj

mrproper:
	rm -rf $(DIRS)

.PHONY: clean mrproper

# will create all necessary directories after the Makefile is parsed
$(shell mkdir -p $(DIRS))
