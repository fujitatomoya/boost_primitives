#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>
#include <vector>

using namespace boost::interprocess;
int main (void)
{
  try {
    // Open a message queue.
    message_queue mq (open_only
                      ,"message_queue" // name
      );

    unsigned int priority;
    message_queue::size_type recvd_size;

		// Receive 100 numbers
    for(int i = 0; i < 100; ++i) {
      int number;
      mq.receive(&number, sizeof(number), recvd_size, priority);
      printf("Received message %d\n", number);
      if(recvd_size != sizeof(number))
        return EXIT_FAILURE;
    }
  }
  catch(interprocess_exception &ex) {
    message_queue::remove("message_queue");
    std::cout << ex.what() << std::endl;
    return EXIT_FAILURE;
  }

  message_queue::remove("message_queue");
  return EXIT_SUCCESS;
}
