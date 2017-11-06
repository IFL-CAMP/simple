#include "Subscriber.h"



std::unique_ptr<SIMPLE::POSITION> simple::Subscriber<SIMPLE::POSITION>::subscribe()
{
	/// Creates a message from the data received through the socket. The message type depends on the template type of the
	/// class instance

	std::unique_ptr<SIMPLE::POSITION> BASEmsg = retrieveBaseMsg();

	// From the class instance, we know the type of the message received. Retrieve the data and header from it
	std::pair<std::unique_ptr<SIMPLE::POSITION>, std::unique_ptr<SIMPLE::HEADER>> result;

	result.first = std::make_unique<SIMPLE::POSITION>(BASEmsg->position());
	result.second = std::make_unique<SIMPLE::HEADER>(BASEmsg->header());

	return result;
}

std::unique_ptr<SIMPLE::GENERIC> simple::Subscriber<SIMPLE::GENERIC>::subscribe()
{
	/// Creates a message from the data received through the socket. The message type depends on the template type of the
	/// class instance

	std::unique_ptr<SIMPLE::GENERIC> BASEmsg = retrieveBaseMsg();

	// From the class instance, we know the type of the message received. Retrieve the data and header from it
	std::pair<std::unique_ptr<SIMPLE::GENERIC>, std::unique_ptr<SIMPLE::HEADER>> result;

	result.first = std::make_unique<SIMPLE::GENERIC>(BASEmsg->gener());
	result.second = std::make_unique<SIMPLE::HEADER>(BASEmsg->header());

	return result;
}
std::unique_ptr<SIMPLE::STATUS> simple::Subscriber<SIMPLE::STATUS>::subscribe()
{
	/// Creates a message from the data received through the socket. The message type depends on the template type of the
	/// class instance

	std::unique_ptr<SIMPLE::STATUS> BASEmsg = retrieveBaseMsg();

	// From the class instance, we know the type of the message received. Retrieve the data and header from it
	std::pair<std::unique_ptr<SIMPLE::STATUS>, std::unique_ptr<SIMPLE::HEADER>> result;

	result.first = std::make_unique<SIMPLE::STATUS>(BASEmsg->status());
	result.second = std::make_unique<SIMPLE::HEADER>(BASEmsg->header());

	return result;
}

std::unique_ptr<SIMPLE::CAPABILITY> simple::Subscriber<SIMPLE::CAPABILITY>::subscribe()
{
	/// Creates a message from the data received through the socket. The message type depends on the template type of the
	/// class instance

	std::unique_ptr<SIMPLE::CAPABILITY> BASEmsg = retrieveBaseMsg();

	// From the class instance, we know the type of the message received. Retrieve the data and header from it
	std::pair<std::unique_ptr<SIMPLE::CAPABILITY>, std::unique_ptr<SIMPLE::HEADER>> result;

	result.first = std::make_unique<SIMPLE::CAPABILITY>(BASEmsg->capability());
	result.second = std::make_unique<SIMPLE::HEADER>(BASEmsg->header());

	return result;
}

std::unique_ptr<SIMPLE::TRANSFORM> simple::Subscriber<SIMPLE::TRANSFORM>::subscribe()
{
	/// Creates a message from the data received through the socket. The message type depends on the template type of the
	/// class instance

	std::unique_ptr<SIMPLE::TRANSFORM> BASEmsg = retrieveBaseMsg();

	// From the class instance, we know the type of the message received. Retrieve the data and header from it
	std::pair<std::unique_ptr<SIMPLE::TRANSFORM>, std::unique_ptr<SIMPLE::HEADER>> result;

	result.first = std::make_unique<SIMPLE::TRANSFORM>(BASEmsg->transform());
	result.second = std::make_unique<SIMPLE::HEADER>(BASEmsg->header());

	return result;
}

std::unique_ptr<SIMPLE::IMAGE> simple::Subscriber<SIMPLE::IMAGE>::subscribe()
{
	/// Creates a message from the data received through the socket. The message type depends on the template type of the
	/// class instance

	std::unique_ptr<SIMPLE::IMAGE> BASEmsg = retrieveBaseMsg();

	// From the class instance, we know the type of the message received. Retrieve the data and header from it
	std::pair<std::unique_ptr<SIMPLE::IMAGE>, std::unique_ptr<SIMPLE::HEADER>> result;

	result.first = std::make_unique<SIMPLE::IMAGE>(BASEmsg->image());
	result.second = std::make_unique<SIMPLE::HEADER>(BASEmsg->header());

	return result;
}