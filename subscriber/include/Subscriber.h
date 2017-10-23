#pragma once

#include <string>
#include "SIMPLE.pb.h"
#include <zmq.hpp>
#include <zmq_utils.h>
#include <memory>
#include <vector>


class Subscriber{
public:

	Subscriber(std::string port);
	~Subscriber();
	std::unique_ptr<SIMPLE::BASEMSG> subscribe();

private:

	std::unique_ptr<zmq::socket_t> socket;
	std::unique_ptr<zmq::context_t> context;

};

