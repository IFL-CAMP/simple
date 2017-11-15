#pragma once

#include <chrono>
#include <memory>
#include "simple_msgs/simple.pb.h"

namespace simple {
class MSGcreator {
 public:
  ///@brief Creates a message of type TRANSFORM
  ///@param header pointer to the header of the message
  ///@param px element e14 of 4x4 transformation matrix
  ///@param py element e24 of 4x4 transformation matrix
  ///@param pz element e34 of 4x4 transformation matrix
  ///@param r11 element e11 of 4x4 transformation matrix
  ///@param r12 element e12 of 4x4 transformation matrix
  ///@param r13 element e13 of 4x4 transformation matrix
  ///@param r21 element e21 of 4x4 transformation matrix
  ///@param r22 element e22 of 4x4 transformation matrix
  ///@param r23 element e23 of 4x4 transformation matrix
  ///@param r31 element e31 of 4x4 transformation matrix
  ///@param r32 element e32 of 4x4 transformation matrix
  ///@param r33 element e33 of 4x4 transformation matrix
  ///@return pointer to message of type TRANSFORM
  std::unique_ptr<simple::transform> createTRANSFORM(
      simple::header* header, double px, double py, double pz, double r11,
      double r12, double r13, double r21, double r22, double r23, double r31,
      double r32, double r33);
  ///@brief Creates a message of type POSITION
  ///@param header pointer to the header of the message
  ///@param px first element of 3D vector for the position
  ///@param py second element of 3D vector for the position
  ///@param pz third element of 3D vector for the position
  ///@param e1 first quaternion: e1 = k_x*sin(theta/2)
  ///@param e2 second quaternion: e2 = k_y*sin(theta/2)
  ///@param e3 third quaternion: e3 = k_z*sin(theta/2)
  ///@param e4 fourth quaternion: e4 = cos(theta/2)
  ///@return pointer to message of type POSITION
  std::unique_ptr<simple::position> createPOSITION(simple::header* header,
                                                   double px, double py,
                                                   double pz, double e1,
                                                   double e2, double e3,
                                                   double e4);
  ///@brief Creates a message of type STATUS
  ///@param header pointer to the header of the message
  ///@param code device status code - 0 to 19
  ///@param subcode device-specific code, defined by developer
  ///@param errorName Name of the error
  ///@param errorMsg Message detailing the error
  ///@return pointer to message of type STATUS
  std::unique_ptr<simple::status> createSTATUS(simple::header* header, int code,
                                               int subcode,
                                               std::string errorName,
                                               std::string errorMsg);
  ///@brief Creates a message of type CAPABILITY
  ///@param header pointer to the header of the message
  ///@param msgNames Vector containing the name of the types of messages
  /// supported by the device
  ///@return pointer to message of type CAPABILITY
  std::unique_ptr<simple::capability> createCAPABILITY(
      simple::header* header, std::vector<std::string> msgNames);
  ///@brief Creates a message of type GENERIC containing a BOOL
  ///@param header pointer to the header of the message
  ///@param data bool to be sent in the message
  ///@return pointer to message of type GENERIC
  std::unique_ptr<simple::generic> createGENERIC_BOOL(simple::header* header,
                                                      bool data);
  ///@brief Creates a message of type GENERIC containing a INT
  ///@param header pointer to the header of the message
  ///@param data int to be sent in the message
  ///@return pointer to message of type GENERIC
  std::unique_ptr<simple::generic> createGENERIC_INT(simple::header* header,
                                                     int data);
  ///@brief Creates a message of type GENERIC containing a FLOAT
  ///@param header pointer to the header of the message
  ///@param data float to be sent in the message
  ///@return pointer to message of type GENERIC
  std::unique_ptr<simple::generic> createGENERIC_FLOAT(simple::header* header,
                                                       float data);
  ///@brief Creates a message of type GENERIC containing a DOUBLE
  ///@param header pointer to the header of the message
  ///@param data double to be sent in the message
  ///@return pointer to message of type GENERIC
  std::unique_ptr<simple::generic> createGENERIC_DOUBLE(simple::header* header,
                                                        double data);
  ///@brief Creates a message of type GENERIC containing a STRING
  ///@param header pointer to the header of the message
  ///@param data string to be sent in the message
  ///@return pointer to message of type GENERIC
  std::unique_ptr<simple::generic> createGENERIC_STR(simple::header* header,
                                                     std::string data);

  simple::header* createHEADER(int versionNum, std::string dataTypeName,
                               std::string deviceName);

 private:
  double getCurrentTime();
};

}  // namespace simple
