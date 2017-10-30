#pragma once

#include <string>
#include "SIMPLE.pb.h"
#include <zmq.hpp>
#include <zmq_utils.h>
#include <memory>
#include <vector>

namespace simple{

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