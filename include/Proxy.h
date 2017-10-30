#pragma once

#include <string>
#include "SIMPLE.pb.h"
#include <zmq.hpp>
#include <zmq_utils.h>
#include <memory>
#include <vector>

namespace simple{

	class Proxy{
		///Proxy class. 
	public:
		Proxy(std::string backport, std::string frontport, zmq::context_t& context);
		~Proxy();

	private:

		std::unique_ptr<zmq::socket_t> frontend;
		std::unique_ptr<zmq::socket_t> backend;

	};
	

}