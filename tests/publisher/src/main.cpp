#include <signal.h>
#include <fstream>
#include <iostream>
#include <string>
#include "Publisher.h"
#include "myContext.h"
#include "simple_msgs/simple.pb.h"
#include "MSGcreator.h"

// handle interruptions
static int s_interrupted = 0;
static void s_signal_handler(int signal_value) { s_interrupted = 1; }
static void s_catch_signals() {
  // struct signal action;
  // action.sa_handler = s_signal_handler;
  // action.sa_flags = 0;
  // sigemptyset(&action.sa_mask);
  signal(SIGINT, s_signal_handler);
  signal(SIGTERM, s_signal_handler);
}

int main(int argc, char* argv[]) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;


  simple::header* dyn_h = new simple::header();
  dyn_h->set_datatypename("header data");
  dyn_h->set_devicename("header device");
  
  const std::string s = "G data";

  auto g = makeGenericUnique(*dyn_h,s);

  std::cout << "G data: " << g->basicstring() << "   " << g->head().devicename() << std::endl;


  simple::header* dyn_h2 = new simple::header();
  dyn_h2->set_datatypename("header data2");
  dyn_h2->set_devicename("header device2");

  const std::string s2 = "G data2";

  simple::generic* g2 = makeGenericRaw(*dyn_h2, s2);

  std::cout << "G data2: " << g2->basicstring() <<"   "<<g2->head().devicename()<< std::endl;

  delete g2,dyn_h2,dyn_h;

  return 0;
}
