CC = g++
CFLAGS = -std=c++11
LIBS = -lpthread -lrt -lboost_system -lboost_filesystem

all: shared_memory_pool shm_vs_heap shm_mapped_file msgq_sernder msgq_receiver managed_mapped_file

shared_memory_pool: ./src/shared_memory_pool.cpp
	$(CC) $(CFLAGS) -o shared_memory_pool ./src/shared_memory_pool.cpp $(LIBS)

shm_vs_heap: ./src/shm_vs_heap.cpp
	$(CC) $(CFLAGS) -o shm_vs_heap ./src/shm_vs_heap.cpp $(LIBS)

shm_mapped_file: ./src/shm_mapped_file.cpp
	$(CC) $(CFLAGS) -o shm_mapped_file ./src/shm_mapped_file.cpp $(LIBS)

managed_mapped_file: ./src/managed_mapped_file.cpp
	$(CC) $(CFLAGS) -o managed_mapped_file ./src/managed_mapped_file.cpp $(LIBS)

msgq_sernder: ./src/msgq_sender.cpp
	$(CC) $(CFLAGS) -o msgq_sender ./src/msgq_sender.cpp $(LIBS)

msgq_receiver: ./src/msgq_receiver.cpp
	$(CC) $(CFLAGS) -o msgq_receiver ./src/msgq_receiver.cpp $(LIBS)

run:
	./shared_memory_pool
	./shm_mapped_file
	./managed_mapped_file

clean:
	rm -f ./shared_memory_pool
	rm -f ./shm_vs_heap
	rm -f ./shm_mapped_file
	rm -f ./managed_mapped_file
	rm -f ./msgq_sender
	rm -f ./msgq_receiver
