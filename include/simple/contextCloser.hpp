#pragma once

#include <zmq.h>
#include <memory>
#include <iostream>

void contextDeleter(void* ctx){
		zmq_ctx_term(ctx);
	}
