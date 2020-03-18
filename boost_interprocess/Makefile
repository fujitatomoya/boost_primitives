CC = g++
CFLAGS = -std=c++11
LIBS = -lpthread -lrt

all: shared_memory_pool shm_vs_heap msgq_sernder msgq_receiver

shared_memory_pool: ./src/shared_memory_pool.cpp
	$(CC) $(CFLAGS) -o shared_memory_pool ./src/shared_memory_pool.cpp $(LIBS)

shm_vs_heap: ./src/shm_vs_heap.cpp
	$(CC) $(CFLAGS) -o shm_vs_heap ./src/shm_vs_heap.cpp $(LIBS)

msgq_sernder: ./src/msgq_sender.cpp
	$(CC) $(CFLAGS) -o msgq_sender ./src/msgq_sender.cpp $(LIBS)

msgq_receiver: ./src/msgq_receiver.cpp
	$(CC) $(CFLAGS) -o msgq_receiver ./src/msgq_receiver.cpp $(LIBS)

run:
	./shared_memory_pool

clean:
	rm -f ./shared_memory_pool
	rm -f ./shm_vs_heap
	rm -f ./msgq_sender
	rm -f ./msgq_receiver
