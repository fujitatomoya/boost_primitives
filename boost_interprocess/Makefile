CC = g++
CFLAGS = -std=c++11
LIBS = -lpthread -lrt

all: shared_memory_pool shm_vs_heap

shared_memory_pool: ./src/shared_memory_pool.cpp
	$(CC) $(CFLAGS) -o shared_memory_pool ./src/shared_memory_pool.cpp $(LIBS)

shm_vs_heap: ./src/shm_vs_heap.cpp
	$(CC) $(CFLAGS) -o shm_vs_heap ./src/shm_vs_heap.cpp $(LIBS)

run:
	./shared_memory_pool

clean:
	rm -f ./shared_memory_pool
	rm -f ./shm_vs_heap
