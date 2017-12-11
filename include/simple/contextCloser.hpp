#pragma once

#include <zmq.h>
#include <memory>
#include <iostream>

void contextDeleter(void* ctx){
		std::cout << "Context closer" << std::endl;
		zmq_ctx_term(ctx);
		std::cout << "Context closed" << std::endl;
		std::cout << "Context deleted" << std::endl;
	}
