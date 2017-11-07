#include "Subscriber.h"

template <>
void simple::Subscriber<simple::TRANSFORM>::readMsg(
    const simple::TRANSFORM& msg, simple::HEADER& header, double& px,
    double& py, double& pz, double& r11, double& r12, double& r13, double& r21,
    double& r22, double& r23, double& r31, double& r32, double& r33) {
  msgReader.readTRANSFORM(msg, header, px, py, pz, r11, r12, r13, r21, r22, r23,
                          r31, r32, r33);
}

template <>
void simple::Subscriber<simple::POSITION>::readMsg(
    const simple::POSITION& msg, simple::HEADER& header, double& px, double& py,
    double& pz, double& e1, double& e2, double& e3, double& e4) {
  msgReader.readPOSITION(msg, header, px, py, pz, e1, e2, e3, e4);
}

template <>
void simple::Subscriber<simple::STATUS>::readMsg(const simple::STATUS& msg,
                                                 simple::HEADER& header,
                                                 int& code, int& subcode,
                                                 std::string& errorName,
                                                 std::string& errorMsg) {
  msgReader.readSTATUS(msg, header, code, subcode, errorName, errorMsg);
}

template <>
void simple::Subscriber<simple::CAPABILITY>::readMsg(
    const simple::CAPABILITY& msg, simple::HEADER& header,
    std::vector<std::string>& msgNames) {
  msgReader.readCAPABILITY(msg, header, msgNames);
}

template <>
void simple::Subscriber<simple::GENERIC>::readMsg(const simple::GENERIC& msg,
                                                  simple::HEADER& header,
                                                  bool& data) {
  msgReader.readGENERIC_BOOL(msg, header, data);
}

template <>
void simple::Subscriber<simple::GENERIC>::readMsg(const simple::GENERIC& msg,
                                                  simple::HEADER& header,
                                                  int& data) {
  msgReader.readGENERIC_INT(msg, header, data);
}

template <>
void simple::Subscriber<simple::GENERIC>::readMsg(const simple::GENERIC& msg,
                                                  simple::HEADER& header,
                                                  float& data) {
  msgReader.readGENERIC_FLOAT(msg, header, data);
}

template <>
void simple::Subscriber<simple::GENERIC>::readMsg(const simple::GENERIC& msg,
                                                  simple::HEADER& header,
                                                  double& data) {
  msgReader.readGENERIC_DOUBLE(msg, header, data);
}

template <>
void simple::Subscriber<simple::GENERIC>::readMsg(const simple::GENERIC& msg,
                                                  simple::HEADER& header,
                                                  std::string& data) {
  msgReader.readGENERIC_STR(msg, header, data);
}
