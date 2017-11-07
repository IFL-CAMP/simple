#include "Server.h"

template <>
std::unique_ptr<simple::TRANSFORM> simple::Server<simple::TRANSFORM>::createMSG(
    simple::HEADER* header, double px, double py, double pz, double r11,
    double r12, double r13, double r21, double r22, double r23, double r31,
    double r32, double r33) {
  /// Creates a message of type TRANSFORM
  std::unique_ptr<simple::TRANSFORM> msg = msgCreator.createTRANSFORM(
      header, px, py, pz, r11, r12, r13, r21, r22, r23, r31, r32, r33);

  return msg;
}

template <>
std::unique_ptr<simple::POSITION> simple::Server<simple::POSITION>::createMSG(
    simple::HEADER* header, double px, double py, double pz, double e1,
    double e2, double e3, double e4) {
  /// Creates a message of type POSITION

  std::unique_ptr<simple::POSITION> msg =
      msgCreator.createPOSITION(header, px, py, pz, e1, e2, e3, e4);

  return msg;
}

template <>
std::unique_ptr<simple::STATUS> simple::Server<simple::STATUS>::createMSG(
    simple::HEADER* header, int code, int subcode, std::string errorName,
    std::string errorMsg) {
  /// Creates a message of type STATUS

  std::unique_ptr<simple::STATUS> msg =
      msgCreator.createSTATUS(header, code, subcode, errorName, errorMsg);

  return msg;
}

template <>
std::unique_ptr<simple::CAPABILITY>
simple::Server<simple::CAPABILITY>::createMSG(
    simple::HEADER* header, std::vector<std::string> msgNames) {
  // Creates a message of type CAPABILITY
  std::unique_ptr<simple::CAPABILITY> msg =
      msgCreator.createCAPABILITY(header, msgNames);

  return msg;
}

template <>
std::unique_ptr<simple::GENERIC> simple::Server<simple::GENERIC>::createMSG(
    simple::HEADER* header, bool data) {
  // Creates a message of type GENERIC - BOOL
  std::unique_ptr<simple::GENERIC> msg =
      msgCreator.createGENERIC_BOOL(header, data);

  return msg;
}

template <>
std::unique_ptr<simple::GENERIC> simple::Server<simple::GENERIC>::createMSG(
    simple::HEADER* header, int data) {
  // Creates a message of type GENERIC - INT
  std::unique_ptr<simple::GENERIC> msg =
      msgCreator.createGENERIC_INT(header, data);

  return msg;
}

template <>
std::unique_ptr<simple::GENERIC> simple::Server<simple::GENERIC>::createMSG(
    simple::HEADER* header, float data) {
  // Creates a message of type GENERIC - FLOAT
  std::unique_ptr<simple::GENERIC> msg =
      msgCreator.createGENERIC_FLOAT(header, data);

  return msg;
}

template <>
std::unique_ptr<simple::GENERIC> simple::Server<simple::GENERIC>::createMSG(
    simple::HEADER* header, double data) {
  // Creates a message of type GENERIC - DOUBLE
  std::unique_ptr<simple::GENERIC> msg =
      msgCreator.createGENERIC_DOUBLE(header, data);

  return msg;
}

template <>
std::unique_ptr<simple::GENERIC> simple::Server<simple::GENERIC>::createMSG(
    simple::HEADER* header, std::string data) {
  // Creates a message of type GENERIC - STRING
  std::unique_ptr<simple::GENERIC> msg =
      msgCreator.createGENERIC_STR(header, data);

  return msg;
}
