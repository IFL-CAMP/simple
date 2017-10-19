#include "Publisher.h"



// Publisher classes for all message types

void Publisher::publish(const SIMPLE::BASEMSG& msg){
	//opens a socket and sends the message by the open socket of the publisher
	
	zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_PUB);
	socket.bind(port);

	std::string strMSG;

	msg.SerializeToString(&strMSG);//serialize the protobuf message into a string

	zmq::message_t ZMQmsg(strMSG.size());
	memcpy(ZMQmsg.data(), &strMSG, strMSG.size());

	socket.send(ZMQmsg);
	
}
Publisher::Publisher(std::string p):port(p){}
Publisher::~Publisher(){
	//close the socket

}
std::unique_ptr<SIMPLE::BASEMSG> Publisher::createTRANSFORM(SIMPLE::HEADER& header, double px, double py, double pz, double r11, double r12, double r13, double r21, double r22, double r23, double r31, double r32, double r33){

	std::unique_ptr<SIMPLE::BASEMSG> msg = std::make_unique<SIMPLE::BASEMSG>();

	SIMPLE::TRANSFORM transform;

	SIMPLE::Pos pos;
	SIMPLE::Orientation orientation;

	pos.set_px(px);
	pos.set_py(py);
	pos.set_pz(pz);

	orientation.set_r11(r11);
	orientation.set_r12(r12);
	orientation.set_r13(r13);
	orientation.set_r21(r21);
	orientation.set_r22(r22);
	orientation.set_r23(r23);
	orientation.set_r31(r31);
	orientation.set_r32(r32);
	orientation.set_r33(r33);

	transform.set_allocated_orient(&orientation);
	transform.set_allocated_position(&pos);

	msg->set_allocated_header(&header);
	msg->set_allocated_transform(&transform);

	return msg;

}
std::unique_ptr<SIMPLE::BASEMSG> Publisher::createPOSITION(SIMPLE::HEADER& header, double px, double py, double pz, double qi, double qj, double qk, double qr){

	std::unique_ptr<SIMPLE::BASEMSG> msg = std::make_unique<SIMPLE::BASEMSG>();

	SIMPLE::POSITION position;

	SIMPLE::Pos pos;
	SIMPLE::Quaternion quaternion;

	quaternion.set_qi(qi);
	quaternion.set_qj(qj);
	quaternion.set_qk(qk);
	quaternion.set_qr(qr);

	pos.set_px(px);
	pos.set_py(py);
	pos.set_pz(pz);

	position.set_allocated_orient(&quaternion);
	position.set_allocated_position(&pos);

	msg->set_allocated_header(&header);
	msg->set_allocated_position(&position);

	return msg;

}
std::unique_ptr<SIMPLE::BASEMSG> Publisher::createSTATUS(SIMPLE::HEADER& header, int code, int subcode, std::string errorName, std::string errorMsg){

	std::unique_ptr<SIMPLE::BASEMSG> msg = std::make_unique<SIMPLE::BASEMSG>();

	SIMPLE::STATUS stat;

	stat.set_subcode(subcode);
	stat.set_statuscode(code);
	stat.set_errormsg(errorMsg);
	stat.set_errorname(errorName);

	msg->set_allocated_header(&header);
	msg->set_allocated_status(&stat);

	return msg;

}
std::unique_ptr<SIMPLE::BASEMSG> Publisher::createCAPABILITY(SIMPLE::HEADER& header, std::vector<std::string> msgNames){

	std::unique_ptr<SIMPLE::BASEMSG> msg = std::make_unique<SIMPLE::BASEMSG>();

	SIMPLE::CAPABILITY cap;

	for (size_t i = 0; i < msgNames.size(); i++)
	{
		cap.add_messagename(msgNames.at(i));
	}

	msg->set_allocated_header(&header);
	msg->set_allocated_capability(&cap);

	return msg;

}
std::unique_ptr<SIMPLE::BASEMSG> Publisher::createGENERIC_BOOL(SIMPLE::HEADER& header, bool data){

	std::unique_ptr<SIMPLE::BASEMSG> msg = std::make_unique<SIMPLE::BASEMSG>();

	SIMPLE::GENERIC gen;
	gen.set_basicbool(data);

	msg->set_allocated_header(&header);
	msg->set_allocated_generic(&gen);

	return msg;

}
std::unique_ptr<SIMPLE::BASEMSG> Publisher::createGENERIC_INT(SIMPLE::HEADER& header, int data){

	std::unique_ptr<SIMPLE::BASEMSG> msg = std::make_unique<SIMPLE::BASEMSG>();

	SIMPLE::GENERIC gen;
	gen.set_basicint(data);

	msg->set_allocated_header(&header);
	msg->set_allocated_generic(&gen);

	return msg;

}
std::unique_ptr<SIMPLE::BASEMSG> Publisher::createGENERIC_FLOAT(SIMPLE::HEADER& header, float data){

	std::unique_ptr<SIMPLE::BASEMSG> msg = std::make_unique<SIMPLE::BASEMSG>();

	SIMPLE::GENERIC gen;
	gen.set_basicfloat(data);

	msg->set_allocated_header(&header);
	msg->set_allocated_generic(&gen);

	return msg;

}
std::unique_ptr<SIMPLE::BASEMSG> Publisher::createGENERIC_DOUBLE(SIMPLE::HEADER& header, double data){

	std::unique_ptr<SIMPLE::BASEMSG> msg = std::make_unique<SIMPLE::BASEMSG>();

	SIMPLE::GENERIC gen;
	gen.set_basicdouble(data);

	msg->set_allocated_header(&header);
	msg->set_allocated_generic(&gen);

	return msg;

}
std::unique_ptr<SIMPLE::BASEMSG> Publisher::createGENERIC_STR(SIMPLE::HEADER& header, std::string data){

	std::unique_ptr<SIMPLE::BASEMSG> msg = std::make_unique<SIMPLE::BASEMSG>();

	SIMPLE::GENERIC gen;
	gen.set_basicstring(data);

	msg->set_allocated_header(&header);
	msg->set_allocated_generic(&gen);

	return msg;

}

std::unique_ptr<SIMPLE::HEADER> Publisher::createHEADER(int versionNum, std::string dataTypeName, std::string deviceName, int timeStamp){

	std::unique_ptr<SIMPLE::HEADER> header = std::make_unique<SIMPLE::HEADER>();
	header->set_datatypename(dataTypeName);
	header->set_devicename(deviceName);
	header->set_timestamp(timeStamp);
	header->set_versionnumber(versionNum);
	return header;

}