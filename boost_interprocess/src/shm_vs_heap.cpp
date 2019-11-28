#include <boost/interprocess/managed_shared_memory.hpp>
#include <chrono>
#include <list>
#include <queue>

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::nanoseconds nanoseconds;

Clock::time_point t0, t1;

using namespace boost::interprocess;

#define BUFFER_SIZE (1024*1024)
#define NUM_BUFFERS (8)
#define CYCLES (1000)

/**
 * Container that suports allocation/deallocation of buffers
 * from a pool in shared-memory
 */
class SharedMemContainer
{
public:

  SharedMemContainer(size_t count)
  {
    shared_memory_object::remove("MySharedMemory");

    // Create a managed shared memory segment
    const size_t allocation_algorithm_structures = 1024; // Extra space needed by the boost internal allocator algorithm
    pool_ = managed_shared_memory (open_or_create, "MySharedMemory", BUFFER_SIZE*NUM_BUFFERS + allocation_algorithm_structures);
  }

  ~SharedMemContainer()
  {
    shared_memory_object::remove("MySharedMemory");
  }
  
  void* alloc()
  {
    return pool_.allocate(BUFFER_SIZE);
  }

  void free(void* buffer)
  {
    pool_.deallocate(buffer);
  }

private:

  managed_shared_memory pool_;
};

/**
 * Container that suports allocation/deallocation of buffers
 * from a pool preallocated in the heap
 */
class HeapContainer
{
public:

  HeapContainer(size_t count)
  {
    pool_ = std::unique_ptr<uint8_t[]>(new uint8_t[BUFFER_SIZE*count]);

    auto p = pool_.get();

    for (size_t i = 0;i<count;i++)
    {
      free_.push_back(p);  
      p+=BUFFER_SIZE;
    }
  }

  void* alloc()
  {
    void* ret = nullptr;

    if((ret = free_.front()))
    {
      used_.push_back(free_.front());
      free_.pop_front();
    }

    return ret;
  }

  void free(void* buffer)
  {
    // We assume allocation/deallocation is FIFO so is done in order
    free_.push_front(used_.front());
    used_.pop_front();
  }

  void* head()
  {
    return used_.front();
  }

private:

  std::list<void*> free_;
  std::list<void*> used_;
  std::unique_ptr<uint8_t[]> pool_;
};

/**
 * Transport interface
 */
class ITransport
{
public:
  virtual void send(void* data) = 0;
  virtual void receive() = 0;
};

/**
 * Transport implementation
 */
template <class Container>
class Transport : public ITransport
{
public:

  Transport(size_t max_buffers)
    : container_(max_buffers)
  {
  }
  void send(void* data) override
  {
    auto transport_buffer = container_.alloc();
    memcpy(transport_buffer, data, BUFFER_SIZE);
    sent_.push(transport_buffer);
  }

  void receive() override
  {
    container_.free(sent_.front());
    sent_.pop();
  }

private:

  Container container_;
  std::queue<void*> sent_;
};

void fill_data(void* data)
{
  memset(data,0,BUFFER_SIZE);
}

/**
 * Perform a transport test 
 * 1) Sends NUM_BUFFERS datagrams, that performs allocations in the underlying container so
 * all memory is used.
 * 2) Receives all buffers so all buffers are deallocated in the underlying container
 * 3) Repeat the operation CYCLES times
 */
void test_transport(ITransport& transport)
{
  auto send_data = std::unique_ptr<uint8_t[]>(new uint8_t[BUFFER_SIZE]);
  auto t0 = Clock::now();

  for(int cycle = 0;cycle < CYCLES;cycle++)
  {
    // Fill the container 
    for(size_t i=0;i<NUM_BUFFERS;i++)
    {
      fill_data(send_data.get());
      transport.send(send_data.get());
    }

    // Empty the container 
    for(size_t i=0;i<NUM_BUFFERS;i++)
    {
      transport.receive();
    }
  }

  auto t1 = Clock::now();

  printf("%u datagrams of %.1f(MB) send/recv in %.3f(ms)\n", NUM_BUFFERS*CYCLES, BUFFER_SIZE/(1024*1024.0), (t1-t0).count()/1000000.0);
}

int main (int argc, char *argv[])
{
  {
    printf("Testing Heap transport...\n");
    Transport<HeapContainer> heap_transport(NUM_BUFFERS);
    test_transport(heap_transport);
  }

  {
    printf("Testing SharedMemory transport...\n");
    Transport<SharedMemContainer> shm_transport(NUM_BUFFERS);
    test_transport(shm_transport);
  }
  
  return EXIT_SUCCESS;
}
