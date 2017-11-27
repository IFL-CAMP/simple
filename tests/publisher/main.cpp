//Test how the publisher works

#include <iostream>
#include "publisher.h"
#include <signal.h>
#include <string>
#include "header.h"

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

	//create the context
	simple::Publisher::context_ = zmq::context_t(1);

	//create a header message, with wrapper
	int seq_n = 1;
	std::string frame_id = "ID";
	double timeStmp = 0.1234;
	simple_msgs::Header h(seq_n, frame_id, timeStmp);

	//create a publisher
	simple::Publisher pub("tcp://*:5555");

	//s_catch_signals();
	int num = 3;
	while (num>0)
	{
		try{//send the message continously
			pub.publish(h);
			std::cout << "Header Message published" << "\n";
		}
		catch (zmq::error_t& e){
			std::cout << "Something went wrong with the publishing..." << "\n";
		}
		num--;
	}

	simple::Publisher::context_.close();

	return 0;
}

