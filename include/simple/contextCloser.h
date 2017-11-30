#pragma once

#include <memory>
#include "zmq.hpp"

struct contextCloser{
	void operator()(zmq::context_t* ctx){
		ctx->close();
		delete ctx;
	}
};