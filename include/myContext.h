#pragma once

#include <zmq.hpp>
#include <zmq_utils.h>
#include <memory>

namespace simple{

	class myContext{
	public:
		std::shared_ptr<zmq::context_t> context;
		myContext(){
			context = std::make_shared<zmq::context_t>(1);
		};
		~myContext(){
			context->close();
		};
	};

}