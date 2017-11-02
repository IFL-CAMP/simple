#pragma once

#include <string>
#include "SIMPLE.pb.h"
#include <zmq.hpp>
#include <zmq_utils.h>
#include <memory>
#include <vector>
#include "MSGcreator.h"

namespace simple{
	template <typename T>
	class Server{
		///Server class. 
	public:
		Server(std::string port, zmq::context_t& context);
		~Server();
		void reply();

	private:

		std::unique_ptr<zmq::socket_t> socket;

	};
	

}
template <typename T>
simple::Server<T>::Server(std::string port, zmq::context_t& context){
	///Class constructor: opens a socket of type REP and connects it to the port

	socket = std::make_unique<zmq::socket_t>(context, ZMQ_REP);

	try{
		socket->connect(port);
	}
	catch (zmq::error_t& e){
		std::cout << "could not bind socket:" << e.what();
	}

}
template <typename T>
simple::Server<T>::~Server(){
	///Class destructor: Closes the socket
	socket->close();
}
void simple::Server<SIMPLE::CAPABILITY>::reply(){

	//wait for next request. Fetch the required data and reply
	zmq::message_t recvREQ;

	try{
		socket->recv(&recvREQ);
	}
	catch (zmq::error_t& e){
		std::cout << "Could not receive message: " << e.what();
	}

	//build protobuf message



}