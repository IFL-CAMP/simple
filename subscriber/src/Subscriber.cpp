#include "Subscriber.h"



std::unique_ptr<SIMPLE::BASEMSG> Subscriber::subscribe(){
	//creates a SIMPLE::BASEMSG from the data received through the socket

	zmq::message_t ZMQmessage;

	try{
		socket->recv(&ZMQmessage);
	}
	catch (zmq::error_t& e){
		std::cout << "Could not receive message: " << e.what();
	}

	std::unique_ptr<SIMPLE::BASEMSG> BASEmsg = std::make_unique<SIMPLE::BASEMSG>();

	std::string strMessage(static_cast<char*>(ZMQmessage.data()), ZMQmessage.size());

	BASEmsg->ParseFromString(strMessage);

	return BASEmsg;

}
Subscriber::Subscriber(std::string port){

	context = std::make_unique<zmq::context_t>(1);
	socket = std::make_unique<zmq::socket_t>(*context, ZMQ_SUB);
	
	try{
		socket->connect(port);
	}
	catch (zmq::error_t& e){
		std::cout << "could not bind to socket:" << e.what();
	}

	//subscribe to all messages
	socket->setsockopt(ZMQ_SUBSCRIBE, "", 0);

}
Subscriber::~Subscriber(){
	//close the socket and destroy the context
	zmq_close(*socket);
	zmq_ctx_destroy(*context);
}