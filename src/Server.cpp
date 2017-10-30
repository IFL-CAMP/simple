#include "Server.h"



simple::Server::Server(std::string port, zmq::context_t& context){
	///Class constructor: opens two sockets, one of type ROUTER and the other of type DEALER
	///Binds both ports to the front and back ports

	socket = std::make_unique<zmq::socket_t>(context, ZMQ_REP);

	try{
		socket->bind(port);
	}
	catch (zmq::error_t& e){
		std::cout << "could not bind frontend socket:" << e.what();
	}

}
simple::Server::~Server(){
	///Class destructor: Closes the sockets
	socket->close();
}