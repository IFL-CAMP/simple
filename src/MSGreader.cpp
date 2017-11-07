#include "MSGreader.h"

void simple::MSGreader::readTRANSFORM(const simple::TRANSFORM& msg,
                                      simple::HEADER& header, double& px,
                                      double& py, double& pz, double& r11,
                                      double& r12, double& r13, double& r21,
                                      double& r22, double& r23, double& r31,
                                      double& r32, double& r33) {
  /// Reads a message of type TRANSFORM

  simple::Pos pos = msg.position();
  simple::Orientation orientation = msg.orient();

  px = pos.px();
  py = pos.py();
  pz = pos.pz();

  r11 = orientation.r11();
  r12 = orientation.r12();
  r13 = orientation.r13();
  r21 = orientation.r21();
  r22 = orientation.r22();
  r23 = orientation.r23();
  r31 = orientation.r31();
  r32 = orientation.r32();
  r33 = orientation.r33();

  header = msg.header();
}
void simple::MSGreader::readPOSITION(const simple::POSITION& msg,
                                     simple::HEADER& header, double& px,
                                     double& py, double& pz, double& e1,
                                     double& e2, double& e3, double& e4) {
  /// Reads a message of type POSITION

  simple::Pos pos = msg.position();
  simple::Quaternion quaternion = msg.orient();

  header = msg.header();
  px = pos.px();
  py = pos.py();
  pz = pos.pz();

  e1 = quaternion.e1();
  e2 = quaternion.e2();
  e3 = quaternion.e3();
  e4 = quaternion.e4();
}
void simple::MSGreader::readSTATUS(const simple::STATUS& msg,
                                   simple::HEADER& header, int& code,
                                   int& subcode, std::string& errorName,
                                   std::string& errorMsg) {
  /// Reads a message of type STATUS

  header = msg.header();
  code = msg.statuscode();
  subcode = msg.subcode();
  errorName = msg.errorname();
  errorMsg = msg.errormsg();
}
void simple::MSGreader::readCAPABILITY(const simple::CAPABILITY& msg,
                                       simple::HEADER& header,
                                       std::vector<std::string>& msgNames) {
  header = msg.header();

  for (int i = 0; i < msg.messagename_size(); i++) {
    msgNames.push_back(msg.messagename(i));
  }
}
void simple::MSGreader::readGENERIC_BOOL(const simple::GENERIC& msg,
                                         simple::HEADER& header, bool& data) {
  header = msg.header();
  data = msg.basicbool();
}
void simple::MSGreader::readGENERIC_INT(const simple::GENERIC& msg,
                                        simple::HEADER& header, int& data) {
  header = msg.header();
  data = msg.basicint();
}
void simple::MSGreader::readGENERIC_FLOAT(const simple::GENERIC& msg,
                                          simple::HEADER& header, float& data) {
  header = msg.header();
  data = msg.basicfloat();
}
void simple::MSGreader::readGENERIC_DOUBLE(const simple::GENERIC& msg,
                                           simple::HEADER& header,
                                           double& data) {
  header = msg.header();
  data = msg.basicdouble();
}
void simple::MSGreader::readGENERIC_STR(const simple::GENERIC& msg,
                                        simple::HEADER& header,
                                        std::string& data) {
  header = msg.header();
  data = msg.basicstring();
}

void simple::MSGreader::readHEADER(const simple::HEADER& header,
                                   int& versionNum, std::string& dataTypeName,
                                   std::string& deviceName, double& timeStamp) {
  /// Reads the message header
  versionNum = header.versionnumber();
  dataTypeName = header.datatypename();
  deviceName = header.devicename();
  timeStamp = header.timestamp();
}
