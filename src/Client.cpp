#include "Client.h"



simple::Client::Client(std::string port, zmq::context_t& context){
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
simple::Client::~Client(){
	///Class destructor: Closes the sockets
	socket->close();
}