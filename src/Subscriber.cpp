#include "Subscriber.h"



std::unique_ptr<SIMPLE::POSITION> simple::Subscriber<SIMPLE::POSITION>::subscribe()
{
	/// Creates a message from the data received through the socket. The message type depends on the template type of the
	/// class instance

	std::unique_ptr<SIMPLE::POSITION> msg = retrieveBaseMsg();


	return msg;
}

std::unique_ptr<SIMPLE::GENERIC> simple::Subscriber<SIMPLE::GENERIC>::subscribe()
{
	/// Creates a message from the data received through the socket. The message type depends on the template type of the
	/// class instance

	std::unique_ptr<SIMPLE::GENERIC> msg = retrieveBaseMsg();

	return msg;
}
std::unique_ptr<SIMPLE::STATUS> simple::Subscriber<SIMPLE::STATUS>::subscribe()
{
	/// Creates a message from the data received through the socket. The message type depends on the template type of the
	/// class instance

	std::unique_ptr<SIMPLE::STATUS> msg = retrieveBaseMsg();

	return msg;
}

std::unique_ptr<SIMPLE::CAPABILITY> simple::Subscriber<SIMPLE::CAPABILITY>::subscribe()
{
	/// Creates a message from the data received through the socket. The message type depends on the template type of the
	/// class instance

	std::unique_ptr<SIMPLE::CAPABILITY> msg = retrieveBaseMsg();

	return msg;
}

std::unique_ptr<SIMPLE::TRANSFORM> simple::Subscriber<SIMPLE::TRANSFORM>::subscribe()
{
	/// Creates a message from the data received through the socket. The message type depends on the template type of the
	/// class instance

	std::unique_ptr<SIMPLE::TRANSFORM> msg = retrieveBaseMsg();

	return msg;
}

std::unique_ptr<SIMPLE::IMAGE> simple::Subscriber<SIMPLE::IMAGE>::subscribe()
{
	/// Creates a message from the data received through the socket. The message type depends on the template type of the
	/// class instance

	std::unique_ptr<SIMPLE::IMAGE> msg = retrieveBaseMsg();

	return msg;
}