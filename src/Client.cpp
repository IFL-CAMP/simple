#include "Client.h"

void simple::Client<SIMPLE::TRANSFORM>::readMsg(const SIMPLE::TRANSFORM& msg, SIMPLE::HEADER& header, double& px,
                                                double& py, double& pz, double& r11, double& r12, double& r13,
                                                double& r21, double& r22, double& r23, double& r31, double& r32,
                                                double& r33)
{
  msgReader.readTRANSFORM(msg, header, px, py, pz, r11, r12, r13, r21, r22, r23, r31, r32, r33);
}
void simple::Client<SIMPLE::POSITION>::readMsg(const SIMPLE::POSITION& msg, SIMPLE::HEADER& header, double& px,
                                               double& py, double& pz, double& e1, double& e2, double& e3, double& e4)
{
  msgReader.readPOSITION(msg, header, px, py, pz, e1, e2, e3, e4);
}
void simple::Client<SIMPLE::STATUS>::readMsg(const SIMPLE::STATUS& msg, SIMPLE::HEADER& header, int& code, int& subcode,
                                             std::string& errorName, std::string& errorMsg)
{
  msgReader.readSTATUS(msg, header, code, subcode, errorName, errorMsg);
}
void simple::Client<SIMPLE::CAPABILITY>::readMsg(const SIMPLE::CAPABILITY& msg, SIMPLE::HEADER& header,
                                                 std::vector<std::string>& msgNames)
{
  msgReader.readCAPABILITY(msg, header, msgNames);
}
void simple::Client<SIMPLE::GENERIC>::readMsg(const SIMPLE::GENERIC& msg, SIMPLE::HEADER& header, bool& data)
{
  msgReader.readGENERIC_BOOL(msg, header, data);
}
void simple::Client<SIMPLE::GENERIC>::readMsg(const SIMPLE::GENERIC& msg, SIMPLE::HEADER& header, int& data)
{
  msgReader.readGENERIC_INT(msg, header, data);
}
void simple::Client<SIMPLE::GENERIC>::readMsg(const SIMPLE::GENERIC& msg, SIMPLE::HEADER& header, float& data)
{
  msgReader.readGENERIC_FLOAT(msg, header, data);
}
void simple::Client<SIMPLE::GENERIC>::readMsg(const SIMPLE::GENERIC& msg, SIMPLE::HEADER& header, double& data)
{
  msgReader.readGENERIC_DOUBLE(msg, header, data);
}
void simple::Client<SIMPLE::GENERIC>::readMsg(const SIMPLE::GENERIC& msg, SIMPLE::HEADER& header, std::string& data)
{
  msgReader.readGENERIC_STR(msg, header, data);
}
