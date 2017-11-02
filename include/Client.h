#pragma once

#include <string>
#include "SIMPLE.pb.h"
#include <zmq.hpp>
#include <zmq_utils.h>
#include <memory>
#include <vector>

namespace simple{
	template <typename T>
	class Client{
		///Proxy class. 
	public:
		Client(std::string port, zmq::context_t& context);
		~Client();
		T request();

	private:

		std::unique_ptr<zmq::socket_t> socket;

	};
	
}
template <typename T>
simple::Client<T>::Client(std::string port, zmq::context_t& context){
	///Class constructor: opens the socket, of type ZMQ_REQ
	///Binds socket to port

	socket = std::make_unique<zmq::socket_t>(context, ZMQ_REQ);

	try{
		socket->connect(port);
	}
	catch (zmq::error_t& e){
		std::cout << "could not bind socket:" << e.what();
	}


}
template <typename T>
simple::Client<T>::~Client(){
	///Class destructor: Closes the socket
	socket->close();
}

SIMPLE::CAPABILITY simple::Client<SIMPLE::CAPABILITY>::request(){

	//send a request to the server and wait for a reply

	std::string msgType = "CAP";

	zmq::message_t ZMQmsg(msgType.size());

	memcpy(ZMQmsg.data(), msgType.c_str(), msgType.size());

	socket->send(ZMQmsg);

	//get reply
	zmq::message_t MSGreply;
	SIMPLE::CAPABILITY SIMPLEreply;

	try{
		socket->recv(&MSGreply);
	}
	catch (zmq::error_t& e){
		std::cout << "Could not receive message: " << e.what();
	}

	std::string strMessage(static_cast<char*>(MSGreply.data()), MSGreply.size());//copy data from ZMQ message into string

	SIMPLEreply.ParseFromString(strMessage);//copy data from string to protobuf message

	return SIMPLEreply;
}