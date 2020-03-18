#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>
#include <vector>

using namespace boost::interprocess;
int main (void)
{
  try {
    // Erase previous message queue
    message_queue::remove("message_queue");

    // Create a message_queue.
    message_queue mq (open_or_create
                      ,"message_queue" // name
                      ,100 // max message number
                      ,sizeof(int) // max message size
      );

		// Send 100 numbers
    for (int i = 0; i < 100; ++i) {
      printf("Sending message %d\n", i);
      mq.send(&i, sizeof(i), 0);
      sleep(1);
    }
  }
  catch(interprocess_exception &ex) {
    std::cout << ex.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
