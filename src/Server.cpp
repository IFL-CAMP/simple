#include "Server.h"

std::unique_ptr<SIMPLE::TRANSFORM> simple::Server<SIMPLE::TRANSFORM>::createMSG(SIMPLE::HEADER* header, double px,
                                                                                double py, double pz, double r11,
                                                                                double r12, double r13, double r21,
                                                                                double r22, double r23, double r31,
                                                                                double r32, double r33)
{
  /// Creates a message of type TRANSFORM
  std::unique_ptr<SIMPLE::TRANSFORM> msg =
      msgCreator.createTRANSFORM(header, px, py, pz, r11, r12, r13, r21, r22, r23, r31, r32, r33);

  return msg;
}
std::unique_ptr<SIMPLE::POSITION> simple::Server<SIMPLE::POSITION>::createMSG(SIMPLE::HEADER* header, double px,
                                                                              double py, double pz, double e1,
                                                                              double e2, double e3, double e4)
{
  /// Creates a message of type POSITION

  std::unique_ptr<SIMPLE::POSITION> msg = msgCreator.createPOSITION(header, px, py, pz, e1, e2, e3, e4);

  return msg;
}
std::unique_ptr<SIMPLE::STATUS> simple::Server<SIMPLE::STATUS>::createMSG(SIMPLE::HEADER* header, int code, int subcode,
                                                                          std::string errorName, std::string errorMsg)
{
  /// Creates a message of type STATUS

  std::unique_ptr<SIMPLE::STATUS> msg = msgCreator.createSTATUS(header, code, subcode, errorName, errorMsg);

  return msg;
}
std::unique_ptr<SIMPLE::CAPABILITY> simple::Server<SIMPLE::CAPABILITY>::createMSG(SIMPLE::HEADER* header,
                                                                                  std::vector<std::string> msgNames)
{
  // Creates a message of type CAPABILITY
  std::unique_ptr<SIMPLE::CAPABILITY> msg = msgCreator.createCAPABILITY(header, msgNames);

  return msg;
}
std::unique_ptr<SIMPLE::GENERIC> simple::Server<SIMPLE::GENERIC>::createMSG(SIMPLE::HEADER* header, bool data)
{
  // Creates a message of type GENERIC - BOOL
  std::unique_ptr<SIMPLE::GENERIC> msg = msgCreator.createGENERIC_BOOL(header, data);

  return msg;
}
std::unique_ptr<SIMPLE::GENERIC> simple::Server<SIMPLE::GENERIC>::createMSG(SIMPLE::HEADER* header, int data)
{
  // Creates a message of type GENERIC - INT
  std::unique_ptr<SIMPLE::GENERIC> msg = msgCreator.createGENERIC_INT(header, data);

  return msg;
}
std::unique_ptr<SIMPLE::GENERIC> simple::Server<SIMPLE::GENERIC>::createMSG(SIMPLE::HEADER* header, float data)
{
  // Creates a message of type GENERIC - FLOAT
  std::unique_ptr<SIMPLE::GENERIC> msg = msgCreator.createGENERIC_FLOAT(header, data);

  return msg;
}
std::unique_ptr<SIMPLE::GENERIC> simple::Server<SIMPLE::GENERIC>::createMSG(SIMPLE::HEADER* header, double data)
{
  // Creates a message of type GENERIC - DOUBLE
  std::unique_ptr<SIMPLE::GENERIC> msg = msgCreator.createGENERIC_DOUBLE(header, data);

  return msg;
}
std::unique_ptr<SIMPLE::GENERIC> simple::Server<SIMPLE::GENERIC>::createMSG(SIMPLE::HEADER* header, std::string data)
{
  // Creates a message of type GENERIC - STRING
  std::unique_ptr<SIMPLE::GENERIC> msg = msgCreator.createGENERIC_STR(header, data);

  return msg;
}
