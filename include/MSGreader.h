#pragma once

#include "simple_msgs/simple.pb.h"
#include <memory>

namespace simple
{
///@brief Class for reading Protobuf messages and returning their content
class MSGreader
{
public:
  ///@brief Reads the content of the input message and returns the values through the reference inputs
  ///@param msg Pointer to the message whose content will be read
  ///@param header return by reference, the header of the message
  ///@param px Return by reference, element e14 of 4x4 transformation matrix
  ///@param py Return by reference, element e24 of 4x4 transformation matrix
  ///@param pz Return by reference, element e34 of 4x4 transformation matrix
  ///@param r11 Return by reference, element e11 of 4x4 transformation matrix
  ///@param r12 Return by reference, element e12 of 4x4 transformation matrix
  ///@param r13 Return by reference, element e13 of 4x4 transformation matrix
  ///@param r21 Return by reference, element e21 of 4x4 transformation matrix
  ///@param r22 Return by reference, element e22 of 4x4 transformation matrix
  ///@param r23 Return by reference, element e23 of 4x4 transformation matrix
  ///@param r31 Return by reference, element e31 of 4x4 transformation matrix
  ///@param r32 Return by reference, element e32 of 4x4 transformation matrix
  ///@param r33 Return by reference, element e33 of 4x4 transformation matrix
  void readTRANSFORM(const simple::TRANSFORM& msg, simple::HEADER& header, double& px, double& py, double& pz,
                     double& r11, double& r12, double& r13, double& r21, double& r22, double& r23, double& r31,
                     double& r32, double& r33);
  ///@brief Reads the content of the input message and returns the values through the reference inputs
  ///@param msg Pointer to the message whose content will be read
  ///@param header return by reference, the header of the message
  ///@param px return by reference, the first element of a 3D position vector
  ///@param py return by reference, the second element of a 3D position vector
  ///@param pz return by reference, the third element of a 3D position vector
  ///@param e1 return by reference, first quaternion: e1 = k_x*sin(theta/2)
  ///@param e2 return by reference, second quaternion: e2 = k_y*sin(theta/2)
  ///@param e3 return by reference, third quaternion: e3 = k_z*sin(theta/2)
  ///@param e4 return by reference, fourth quaternion: e4 = cos(theta/2)
  void readPOSITION(const simple::POSITION& msg, simple::HEADER& header, double& px, double& py, double& pz,
                    double& e1, double& e2, double& e3, double& e4);
  ///@brief Reads the content of the input message and returns the values through the reference inputs
  ///@param msg Pointer to the message whose content will be read
  ///@param header return by reference, the header of the message
  ///@param code return by reference, device status code - 0 to 19
  ///@param subcode return by reference, device-specific code, defined by developer
  ///@param errorName return by reference, Name of the error
  ///@param errorMsg return by reference, Message detailing the error
  void readSTATUS(const simple::STATUS& msg, simple::HEADER& header, int& code, int& subcode,
                  std::string& errorName, std::string& errorMsg);
  ///@brief Reads the content of the input message and returns the values through the reference inputs
  ///@param msg Pointer to the message whose content will be read
  ///@param header return by reference, the header of the message
  ///@param msgNames return by reference, Vector containing the name of the types of messages supported by the device
  void readCAPABILITY(const simple::CAPABILITY& msg, simple::HEADER& header,
                      std::vector<std::string>& msgNames);
  ///@brief Reads the content of the input message and returns the values through the reference inputs
  ///@param msg Pointer to the message whose content will be read
  ///@param header return by reference, the header of the message
  ///@param data return by reference, bool content of the message
  void readGENERIC_BOOL(const simple::GENERIC& msg, simple::HEADER& header, bool& data);
  ///@brief Reads the content of the input message and returns the values through the reference inputs
  ///@param msg Pointer to the message whose content will be read
  ///@param header return by reference, the header of the message
  ///@param data return by reference, int content of the message
  void readGENERIC_INT(const simple::GENERIC& msg, simple::HEADER& header, int& data);
  ///@brief Reads the content of the input message and returns the values through the reference inputs
  ///@param msg Pointer to the message whose content will be read
  ///@param header return by reference, the header of the message
  ///@param data return by reference, float content of the message
  void readGENERIC_FLOAT(const simple::GENERIC& msg, simple::HEADER& header, float& data);
  ///@brief Reads the content of the input message and returns the values through the reference inputs
  ///@param msg Pointer to the message whose content will be read
  ///@param header return by reference, the header of the message
  ///@param data return by reference, double content of the message
  void readGENERIC_DOUBLE(const simple::GENERIC& msg, simple::HEADER& header, double& data);
  ///@brief Reads the content of the input message and returns the values through the reference inputs
  ///@param msg Pointer to the message whose content will be read
  ///@param header return by reference, the header of the message
  ///@param data return by reference, string content of the message
  void readGENERIC_STR(const simple::GENERIC& msg, simple::HEADER& header, std::string& data);

  void readHEADER(const simple::HEADER& header, int& versionNum, std::string& dataTypeName, std::string& deviceName,
                  double& timeStamp);
};

}  // namespace simple

