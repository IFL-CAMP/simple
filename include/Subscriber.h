#pragma once

#include <string>
#include "SIMPLE.pb.h"
#include <zmq.hpp>
#include <zmq_utils.h>
#include <memory>
#include <vector>

namespace simple {
	template <typename T>
	class Subscriber{
	public:

		Subscriber(std::string port, zmq::context_t& context);
		~Subscriber();
		std::pair<std::unique_ptr<T>,std::unique_ptr<SIMPLE::HEADER>> subscribe();

		//STATUS_Params getSTATUS(SIMPLE::BASEMSG& msg);
		//POSITION_Params getPOSITION(SIMPLE::BASEMSG& msg);
		//TRANSFORM_Params getTRANSFORM(SIMPLE::BASEMSG& msg);

	private:
		std::unique_ptr<SIMPLE::BASEMSG> retrieveBaseMsg();
		void filterSubscription();
		std::unique_ptr<zmq::socket_t> socket;

	};
	
}

template <typename T>
std::unique_ptr<SIMPLE::BASEMSG> simple::Subscriber<T>::retrieveBaseMsg(){
	///Subscription of ANY type of message. The subscription filter is already set on the instance socket

	zmq::message_t ZMQmessage;

	try{
		socket->recv(&ZMQmessage);//receive messages that fit the filter of the socket
	}
	catch (zmq::error_t& e){
		std::cout << "Could not receive message: " << e.what();
	}

	std::unique_ptr<SIMPLE::BASEMSG> BASEmsg = std::make_unique<SIMPLE::BASEMSG>();

	std::string strMessage(static_cast<char*>(ZMQmessage.data()), ZMQmessage.size());//copy data from ZMQ message into string

	BASEmsg->ParseFromString(strMessage);//copy data from string to protobuf message

	return BASEmsg;

}



void simple::Subscriber<SIMPLE::CAPABILITY>::filterSubscription(){
	///Sets up a filter so the subscription only receives CAPABILITY messages

	//the message type consists of 3 letters: the last 3 on filter have to match the message type
	std::string filterStr = "\n\x3TYP";
	
	socket->setsockopt(ZMQ_SUBSCRIBE, filterStr.c_str(), filterStr.length());

}

void simple::Subscriber<SIMPLE::IMAGE>::filterSubscription(){
	///Sets up a filter so the subscription only receives IMAGE messages

	//the message type consists of 3 letters: the last 3 on filter have to match the message type
	std::string filterStr = "\n\x3IMG";

	socket->setsockopt(ZMQ_SUBSCRIBE, filterStr.c_str(), filterStr.length());

}


void simple::Subscriber<SIMPLE::TRANSFORM>::filterSubscription(){
	///Sets up a filter so the subscription only receives TRANSFORM messages

	//the message type consists of 3 letters: the last 3 on filter have to match the message type
	std::string filterStr = "\n\x3TRF";

	socket->setsockopt(ZMQ_SUBSCRIBE, filterStr.c_str(), filterStr.length());

}

void simple::Subscriber<SIMPLE::GENERIC>::filterSubscription(){
	///Sets up a filter so the subscription only receives GENERIC messages

	//the message type consists of 3 letters: the last 3 on filter have to match the message type
	std::string filterStr = "\n\x3GEN";

	socket->setsockopt(ZMQ_SUBSCRIBE, filterStr.c_str(), filterStr.length());

}

void simple::Subscriber<SIMPLE::POSITION>::filterSubscription(){
	///Sets up a filter so the subscription only receives POSITION messages

	//the message type consists of 3 letters: the last 3 on filter have to match the message type
	std::string filterStr = "\n\x3POS";

	socket->setsockopt(ZMQ_SUBSCRIBE, filterStr.c_str(), filterStr.length());

}

void simple::Subscriber<SIMPLE::STATUS>::filterSubscription(){
	///Sets up a filter so the subscription only receives STATUS messages

	//the message type consists of 3 letters: the last 3 on filter have to match the message type
	std::string filterStr = "\n\x3STT";

	socket->setsockopt(ZMQ_SUBSCRIBE, filterStr.c_str(), filterStr.length());

}


template <typename T>
simple::Subscriber<T>::Subscriber(std::string port, zmq::context_t& context){
	///Constructor: opens a socket of type ZMQ_SUB and connects it to the port. Context shall be provided
	///Sets up a filter for the subscription depending on the template type of the instance

	socket = std::make_unique<zmq::socket_t>(context, ZMQ_SUB);

	try{
		socket->connect(port);
	}
	catch (zmq::error_t& e){
		std::cout << "could not bind to socket:" << e.what();
	}

	//filter the type of messages this subscriber will receive. Filter type depends on the templated instance
	filterSubscription();

}

template <typename T>
simple::Subscriber<T>::~Subscriber(){
	//close the socket and destroy the context
	socket->close();

}




std::pair<std::unique_ptr<SIMPLE::POSITION>, std::unique_ptr<SIMPLE::HEADER>> simple::Subscriber<SIMPLE::POSITION>::subscribe(){
	///Creates a message from the data received through the socket. The message type depends on the template type of the class instance 

	std::unique_ptr<SIMPLE::BASEMSG> BASEmsg = retrieveBaseMsg();

	//From the class instance, we know the type of the message received. Retrieve the data and header from it
	std::pair<std::unique_ptr<SIMPLE::POSITION>, std::unique_ptr<SIMPLE::HEADER>> result;

	result.first = std::make_unique<SIMPLE::POSITION>(BASEmsg->position());
	result.second = std::make_unique<SIMPLE::HEADER>(BASEmsg->header());

	return result;

}

std::pair<std::unique_ptr<SIMPLE::GENERIC>, std::unique_ptr<SIMPLE::HEADER>> simple::Subscriber<SIMPLE::GENERIC>::subscribe(){
	///Creates a message from the data received through the socket. The message type depends on the template type of the class instance 

	std::unique_ptr<SIMPLE::BASEMSG> BASEmsg = retrieveBaseMsg();

	//From the class instance, we know the type of the message received. Retrieve the data and header from it
	std::pair<std::unique_ptr<SIMPLE::GENERIC>, std::unique_ptr<SIMPLE::HEADER>> result;

	result.first = std::make_unique<SIMPLE::GENERIC>(BASEmsg->gener());
	result.second = std::make_unique<SIMPLE::HEADER>(BASEmsg->header());

	return result;

}
std::pair<std::unique_ptr<SIMPLE::STATUS>, std::unique_ptr<SIMPLE::HEADER>> simple::Subscriber<SIMPLE::STATUS>::subscribe(){
	///Creates a message from the data received through the socket. The message type depends on the template type of the class instance 

	std::unique_ptr<SIMPLE::BASEMSG> BASEmsg = retrieveBaseMsg();

	//From the class instance, we know the type of the message received. Retrieve the data and header from it
	std::pair<std::unique_ptr<SIMPLE::STATUS>, std::unique_ptr<SIMPLE::HEADER>> result;

	result.first = std::make_unique<SIMPLE::STATUS>(BASEmsg->status());
	result.second = std::make_unique<SIMPLE::HEADER>(BASEmsg->header());

	return result;

}

std::pair<std::unique_ptr<SIMPLE::CAPABILITY>, std::unique_ptr<SIMPLE::HEADER>> simple::Subscriber<SIMPLE::CAPABILITY>::subscribe(){
	///Creates a message from the data received through the socket. The message type depends on the template type of the class instance 

	std::unique_ptr<SIMPLE::BASEMSG> BASEmsg = retrieveBaseMsg();

	//From the class instance, we know the type of the message received. Retrieve the data and header from it
	std::pair<std::unique_ptr<SIMPLE::CAPABILITY>, std::unique_ptr<SIMPLE::HEADER>> result;

	result.first = std::make_unique<SIMPLE::CAPABILITY>(BASEmsg->capability());
	result.second = std::make_unique<SIMPLE::HEADER>(BASEmsg->header());

	return result;

}


std::pair<std::unique_ptr<SIMPLE::TRANSFORM>, std::unique_ptr<SIMPLE::HEADER>> simple::Subscriber<SIMPLE::TRANSFORM>::subscribe(){
	///Creates a message from the data received through the socket. The message type depends on the template type of the class instance 

	std::unique_ptr<SIMPLE::BASEMSG> BASEmsg = retrieveBaseMsg();

	//From the class instance, we know the type of the message received. Retrieve the data and header from it
	std::pair<std::unique_ptr<SIMPLE::TRANSFORM>, std::unique_ptr<SIMPLE::HEADER>> result;

	result.first = std::make_unique<SIMPLE::TRANSFORM>(BASEmsg->transform());
	result.second = std::make_unique<SIMPLE::HEADER>(BASEmsg->header());

	return result;

}

std::pair<std::unique_ptr<SIMPLE::IMAGE>, std::unique_ptr<SIMPLE::HEADER>> simple::Subscriber<SIMPLE::IMAGE>::subscribe(){
	///Creates a message from the data received through the socket. The message type depends on the template type of the class instance 

	std::unique_ptr<SIMPLE::BASEMSG> BASEmsg = retrieveBaseMsg();

	//From the class instance, we know the type of the message received. Retrieve the data and header from it
	std::pair<std::unique_ptr<SIMPLE::IMAGE>, std::unique_ptr<SIMPLE::HEADER>> result;

	result.first = std::make_unique<SIMPLE::IMAGE>(BASEmsg->image());
	result.second = std::make_unique<SIMPLE::HEADER>(BASEmsg->header());

	return result;

}
