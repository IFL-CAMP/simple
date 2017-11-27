// Test how the publisher works

#include <iostream>
#include "publisher.h"
#include <signal.h>
#include <string>
#include "header.h"
#include <thread>

// handle interruptions
static int s_interrupted = 0;
static void s_signal_handler(int signal_value)
{
  s_interrupted = 1;
}

static void s_catch_signals()
{
  signal(SIGINT, s_signal_handler);
  signal(SIGTERM, s_signal_handler);
}

int main(int argc, char* argv[])
{
  using namespace std::chrono_literals;

  // create a header message, with wrapper
  int seq_n = 1;
  std::string frame_id = "ID";
  double timeStmp = 0.1234;
  simple_msgs::Header h(seq_n, frame_id, timeStmp);

  // create a publisher
  simple::Publisher pub("tcp://*:5555");

  s_catch_signals();

  while (!s_interrupted)
  {
    try
    {  // send the message continously
      pub.publish(h);
      std::cout << "Header Message published" << std::endl;
      std::this_thread::sleep_for(1s);
    }
    catch (zmq::error_t& e)
    {
      std::cout << "Something went wrong with the publishing..." << std::endl;
    }
  }

  return 0;
}
