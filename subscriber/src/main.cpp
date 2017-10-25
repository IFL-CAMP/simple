#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <signal.h>
#include "SIMPLE.pb.h"
#include "Subscriber.h"
#include <google/protobuf/text_format.h>

static int s_interrupted = 0;
static void s_signal_handler(int signal_value)
{
	s_interrupted = 1;
}

static void s_catch_signals(void)
{
	signal(SIGINT, s_signal_handler);
	signal(SIGTERM, s_signal_handler);
}

int main(int argc, char* argv[]) {

	GOOGLE_PROTOBUF_VERIFY_VERSION;

	//create context
	simple::myContext globalContext;
	
	simple::Subscriber<SIMPLE::GENERIC> sub("tcp://localhost:5556", globalContext.context);

	s_catch_signals();
	while (true)
	{
		try{
			
			std::pair<std::unique_ptr<SIMPLE::GENERIC>,std::unique_ptr<SIMPLE::HEADER>> income = sub.subscribe();
			//write the content of the message
			std::string msgSTR;
			
			google::protobuf::TextFormat::PrintToString(*income.first, &msgSTR);
			std::cout << msgSTR << "\n";

		}
		
		catch (zmq::error_t& e){

			std::cout << "Interruption received" << "\n";

		}

		if (s_interrupted)
		{
			std::cout << "Interruption received" << "\n";
			break;
		}

	}

	google::protobuf::ShutdownProtobufLibrary();


	return 0;
}