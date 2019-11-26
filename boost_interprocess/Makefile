CC = g++
CFLAGS = -std=c++11
LIBS = -lpthread -lrt

all: shared_memory_pool

shared_memory_pool: ./src/shared_memory_pool.cpp
	$(CC) $(CFLAGS) -o shared_memory_pool ./src/shared_memory_pool.cpp $(LIBS)

run:
	./shared_memory_pool

clean:
	rm -f ./shared_memory_pool
