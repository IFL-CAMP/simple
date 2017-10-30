#include "Proxy.h"



simple::Proxy::Proxy(std::string backport, std::string frontport, zmq::context_t& context){
	///Class constructor: opens two sockets, one of type ROUTER and the other of type DEALER
	///Binds both ports to the front and back ports

	frontend = std::make_unique<zmq::socket_t>(context, ZMQ_ROUTER);
	backend = std::make_unique<zmq::socket_t>(context, ZMQ_DEALER);

	try{
		frontend->bind(frontport);
	}
	catch (zmq::error_t& e){
		std::cout << "could not bind frontend socket:" << e.what();
	}

	try{
		backend->bind(backport);
	}
	catch (zmq::error_t& e){
		std::cout << "could not bind backend socket:" << e.what();
	}

	//start the proxy
	zmq::proxy(*frontend, *backend, nullptr);

}
simple::Proxy::~Proxy(){
	///Class destructor: Closes the sockets
	frontend->close();
	backend->close();
}