#include <boost/interprocess/managed_shared_memory.hpp>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <cstring>

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::nanoseconds nanoseconds;

Clock::time_point t0, t1;

using namespace boost::interprocess;

#define SIZE_8MB (8*1024*1024)
#define SIZE_4MB (4*1024*1024)

/*
 * arguments:
 *    boolean allocate_once: if true, allocate memory from the segment once, then VA will be re-used for the iteration.
 *                           if faluse, allocate and dis-allocate will be done every single iteration.
 */
void test_shm(bool allocate_once)
{
  printf("--- test with shm ---\n");

  //Remove shared memory on construction and destruction
  struct shm_remove
  {
    shm_remove() {  shared_memory_object::remove("MySharedMemory"); }
    ~shm_remove(){  shared_memory_object::remove("MySharedMemory"); }
  } remover;

  //Create a managed shared memory segment
  managed_shared_memory segment(open_or_create, "MySharedMemory", SIZE_8MB);

  //Allocate a portion of the segment (raw memory)
  managed_shared_memory::size_type free_memory = segment.get_free_memory();
  printf("whole memory size is %ld byte, the reset is %ld\n", free_memory, segment.get_free_memory());

  uint64_t time_sum = 0;
  void * shptr;
  nanoseconds ns_diff = std::chrono::duration_cast<nanoseconds>(t1 - t0);

  if (allocate_once) {
    shptr = segment.allocate(SIZE_4MB);
  }
  for (int i = 0; i < 10; i++) {
    if (!allocate_once) {
      shptr = segment.allocate(SIZE_4MB);
    }
    t0 = Clock::now();
    // insert function here.
    std::memset(shptr, 0, SIZE_4MB);
    t1 = Clock::now();
    if (!allocate_once) {
      segment.deallocate(shptr);
    }
    ns_diff = std::chrono::duration_cast<nanoseconds>(t1 - t0);
    time_sum += ns_diff.count();
    printf("%10ld nano seconds [%d iteration]\n", ns_diff.count(), i);
  }
  if (allocate_once) {
    segment.deallocate(shptr);
  }
  printf("avg time = %.3f(ns)\n",time_sum / 10.0);
}

void test_new()
{
  printf("--- test with new ---\n");

  //Allocate a portion of the segment (raw memory)
  uint8_t* pool = new uint8_t[SIZE_8MB];
  void* shptr = pool;

  uint64_t time_sum = 0;
  nanoseconds ns_diff = std::chrono::duration_cast<nanoseconds>(t1 - t0);
  for (int i = 0; i < 10; i++) {
    t0 = Clock::now();
    // insert function here.
    std::memset(shptr, 0, SIZE_4MB);
    t1 = Clock::now();
    ns_diff = std::chrono::duration_cast<nanoseconds>(t1 - t0);
    time_sum += ns_diff.count();
    printf("%10ld nano seconds [%d iteration]\n", ns_diff.count(), i);
  }

  printf("avg time = %.3f(ns)\n",time_sum / 10.0);

  delete [] pool;
}

int main (int argc, char *argv[])
{
  // test_new();
  test_shm(true);
  test_shm(false);

  return EXIT_SUCCESS;
}
