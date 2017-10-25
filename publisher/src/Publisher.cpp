#include "Publisher.h"



void simple::Publisher::publish(const SIMPLE::BASEMSG& msg){
	///Sends the message by the open socket of the publisher. Any type of message is supported
	
	std::string strMSG;

	msg.SerializeToString(&strMSG);//serialize the protobuf message into a string

	zmq::message_t ZMQmsg(strMSG.size());

	memcpy(ZMQmsg.data(), strMSG.c_str(), strMSG.size());

	try{
		socket->send(ZMQmsg);
	}
	catch (zmq::error_t& e){
		std::cout << "Could not send message: " << e.what();
	}
	
}
simple::Publisher::Publisher(std::string port, zmq::context_t& context){
	///Class constructor: opens a socket of type ZMQ_PUB and binds it to port
	
	socket = std::make_unique<zmq::socket_t>(context, ZMQ_PUB);
	try{
		socket->bind(port);
	}
	catch (zmq::error_t& e){
		std::cout << "could not bind to socket:" << e.what();
	}
	
}
simple::Publisher::~Publisher(){
	///Class destructor: Closes the socket and context
	socket->close();
}
std::unique_ptr<SIMPLE::BASEMSG> simple::Publisher::createTRANSFORM(SIMPLE::HEADER* header, double px, double py, double pz, double r11, double r12, double r13, double r21, double r22, double r23, double r31, double r32, double r33){
	///Creates a message of type TRANSFORM

	std::unique_ptr<SIMPLE::BASEMSG> msg = std::make_unique<SIMPLE::BASEMSG>();

	SIMPLE::TRANSFORM* transform = new SIMPLE::TRANSFORM();

	SIMPLE::Pos* pos = new SIMPLE::Pos();
	SIMPLE::Orientation* orientation = new SIMPLE::Orientation();

	pos->set_px(px);
	pos->set_py(py);
	pos->set_pz(pz);

	orientation->set_r11(r11);
	orientation->set_r12(r12);
	orientation->set_r13(r13);
	orientation->set_r21(r21);
	orientation->set_r22(r22);
	orientation->set_r23(r23);
	orientation->set_r31(r31);
	orientation->set_r32(r32);
	orientation->set_r33(r33);

	transform->set_allocated_orient(orientation);
	transform->set_allocated_position(pos);

	msg->set_allocated_header(header);
	msg->set_allocated_transform(transform);

	return msg;

}
std::unique_ptr<SIMPLE::BASEMSG> simple::Publisher::createPOSITION(SIMPLE::HEADER* header, double px, double py, double pz, double qi, double qj, double qk, double qr){
	///Creates a message of type POSITION

	std::unique_ptr<SIMPLE::BASEMSG> msg = std::make_unique<SIMPLE::BASEMSG>();

	SIMPLE::POSITION* position = new SIMPLE::POSITION();

	SIMPLE::Pos* pos = new SIMPLE::Pos();
	SIMPLE::Quaternion* quaternion = new SIMPLE::Quaternion();

	quaternion->set_qi(qi);
	quaternion->set_qj(qj);
	quaternion->set_qk(qk);
	quaternion->set_qr(qr);

	pos->set_px(px);
	pos->set_py(py);
	pos->set_pz(pz);

	position->set_allocated_orient(quaternion);
	position->set_allocated_position(pos);

	msg->set_allocated_header(header);
	msg->set_allocated_position(position);

	return msg;

}
std::unique_ptr<SIMPLE::BASEMSG> simple::Publisher::createSTATUS(SIMPLE::HEADER* header, int code, int subcode, std::string errorName, std::string errorMsg){
	///Creates a message of type STATUS

	std::unique_ptr<SIMPLE::BASEMSG> msg = std::make_unique<SIMPLE::BASEMSG>();

	SIMPLE::STATUS* stat = new SIMPLE::STATUS();

	stat->set_subcode(subcode);
	stat->set_statuscode(code);
	stat->set_errormsg(errorMsg);
	stat->set_errorname(errorName);

	msg->set_allocated_header(header);
	msg->set_allocated_status(stat);

	return msg;

}
std::unique_ptr<SIMPLE::BASEMSG> simple::Publisher::createCAPABILITY(SIMPLE::HEADER* header, std::vector<std::string> msgNames){

	std::unique_ptr<SIMPLE::BASEMSG> msg = std::make_unique<SIMPLE::BASEMSG>();

	SIMPLE::CAPABILITY* cap = new SIMPLE::CAPABILITY();

	for (size_t i = 0; i < msgNames.size(); i++)
	{
		cap->add_messagename(msgNames.at(i));
	}

	msg->set_allocated_header(header);
	msg->set_allocated_capability(cap);

	return msg;

}
std::unique_ptr<SIMPLE::BASEMSG> simple::Publisher::createGENERIC_BOOL(SIMPLE::HEADER* header, bool data){

	std::unique_ptr<SIMPLE::BASEMSG> msg = std::make_unique<SIMPLE::BASEMSG>();

	SIMPLE::GENERIC* gen = new SIMPLE::GENERIC();
	gen->set_basicbool(data);

	
	msg->set_allocated_header(header);
	msg->set_allocated_gener(gen);

	return msg;

}
std::unique_ptr<SIMPLE::BASEMSG> simple::Publisher::createGENERIC_INT(SIMPLE::HEADER* header, int data){

	std::unique_ptr<SIMPLE::BASEMSG> msg = std::make_unique<SIMPLE::BASEMSG>();
	SIMPLE::GENERIC* gen = new SIMPLE::GENERIC();
	
	gen->set_basicint(data);

	msg->set_allocated_header(header);
	msg->set_allocated_gener(gen);//takes ownership of GENERIC

	return msg;

}
std::unique_ptr<SIMPLE::BASEMSG> simple::Publisher::createGENERIC_FLOAT(SIMPLE::HEADER* header, float data){

	std::unique_ptr<SIMPLE::BASEMSG> msg = std::make_unique<SIMPLE::BASEMSG>();

	SIMPLE::GENERIC* gen = new SIMPLE::GENERIC();
	gen->set_basicfloat(data);

	msg->set_allocated_header(header);
	msg->set_allocated_gener(gen);

	return msg;

}
std::unique_ptr<SIMPLE::BASEMSG> simple::Publisher::createGENERIC_DOUBLE(SIMPLE::HEADER* header, double data){

	std::unique_ptr<SIMPLE::BASEMSG> msg = std::make_unique<SIMPLE::BASEMSG>();

	SIMPLE::GENERIC* gen = new SIMPLE::GENERIC();
	gen->set_basicdouble(data);

	msg->set_allocated_header(header);
	msg->set_allocated_gener(gen);

	return msg;

}
std::unique_ptr<SIMPLE::BASEMSG> simple::Publisher::createGENERIC_STR(SIMPLE::HEADER* header, std::string data){

	std::unique_ptr<SIMPLE::BASEMSG> msg = std::make_unique<SIMPLE::BASEMSG>();
	SIMPLE::GENERIC* gen = new SIMPLE::GENERIC();
	
	gen->set_basicstring(data);
	

	msg->set_allocated_header(header);
	msg->set_allocated_gener(gen);

	return msg;

}

SIMPLE::HEADER* simple::Publisher::createHEADER(int versionNum, std::string dataTypeName, std::string deviceName, double timeStamp){
	///Creates the header of the message, including version number,type of the data, name of the transmiting device and time stamp of the message.

	SIMPLE::HEADER* header = new SIMPLE::HEADER();
	header->set_datatypename(dataTypeName);
	header->set_devicename(deviceName);
	header->set_timestamp(timeStamp);
	header->set_versionnumber(versionNum);
	return header;

}