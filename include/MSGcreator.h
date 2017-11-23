#pragma once

#include <chrono>
#include <memory>
#include "header_generated.h"
#include "position_generated.h"
#include "transform_generated.h"
#include "capability_generated.h"
#include "status_generated.h"
#include "generic_bool_generated.h"
#include "generic_double_generated.h"
#include "generic_int_generated.h"
#include "generic_float_generated.h"
#include "generic_string_generated.h"

namespace simple
{
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
std::unique_ptr<flatbuffers::FlatBufferBuilder> createTRANSFORM(flatbuffers::FlatBufferBuilder header,
                                                                flatbuffers::FlatBufferBuilder vector,
                                                                flatbuffers::FlatBufferBuilder quaternion);
///@brief
///@param x
///@param y
///@param z
///@return
std::unique_ptr<flatbuffers::FlatBufferBuilder> createVec3(double x, double y, double z);
///@brief
///@param x
///@param y
///@param z
///@return
std::unique_ptr<flatbuffers::FlatBufferBuilder> createQuaternion(double e1, double e2, double e3, double e4);
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
std::unique_ptr<flatbuffers::FlatBufferBuilder> createPOSITION(flatbuffers::FlatBufferBuilder header, double px,
                                                               double py, double pz, double e1, double e2, double e3,
                                                               double e4);
///@brief Creates a message of type STATUS
///@param header pointer to the header of the message
///@param code device status code - 0 to 19
///@param subcode device-specific code, defined by developer
///@param errorName Name of the error
///@param errorMsg Message detailing the error
///@return pointer to message of type STATUS
std::unique_ptr<flatbuffers::FlatBufferBuilder> createSTATUS(flatbuffers::FlatBufferBuilder header, int code,
                                                             int subcode, const std::string& errorName,
                                                             const std::string& errorMsg);
///@brief Creates a message of type CAPABILITY
///@param header pointer to the header of the message
///@param msgNames Vector containing the name of the types of messages
/// supported by the device
///@return pointer to message of type CAPABILITY
std::unique_ptr<flatbuffers::FlatBufferBuilder> createCAPABILITY(flatbuffers::FlatBufferBuilder header,
                                                                 const std::vector<std::string>& msgNames);
///@brief Creates a message of type GENERIC containing a BOOL
///@param header pointer to the header of the message
///@param data bool to be sent in the message
///@return pointer to message of type GENERIC
std::unique_ptr<flatbuffers::FlatBufferBuilder> createGENERIC_BOOL(flatbuffers::FlatBufferBuilder header, bool data);
///@brief Creates a message of type GENERIC containing a INT
///@param header pointer to the header of the message
///@param data int to be sent in the message
///@return pointer to message of type GENERIC
std::unique_ptr<flatbuffers::FlatBufferBuilder> createGENERIC_INT(flatbuffers::FlatBufferBuilder header, int data);
///@brief Creates a message of type GENERIC containing a FLOAT
///@param header pointer to the header of the message
///@param data float to be sent in the message
///@return pointer to message of type GENERIC
std::unique_ptr<flatbuffers::FlatBufferBuilder> createGENERIC_FLOAT(flatbuffers::FlatBufferBuilder header, float data);
///@brief Creates a message of type GENERIC containing a DOUBLE
///@param header pointer to the header of the message
///@param data double to be sent in the message
///@return pointer to message of type GENERIC
std::unique_ptr<flatbuffers::FlatBufferBuilder> createGENERIC_DOUBLE(flatbuffers::FlatBufferBuilder header,
                                                                     double data);
///@brief Creates a message of type GENERIC containing a STRING
///@param ptr
///@param header pointer to the header of the message
///@param data string to be sent in the message
///@return pointer to message of type GENERIC
std::unique_ptr<flatbuffers::FlatBufferBuilder> createGENERIC_STR(flatbuffers::FlatBufferBuilder header,
                                                                  const std::string& data);

std::unique_ptr<flatbuffers::FlatBufferBuilder> createHeader(std::string datatypename, std::string devicename,
                                                             int versionnum);
double getCurrentTime();

}  // namespace simple

/*#include "simple_msgs/simple.pb.h"

namespace simple
{
  

  void createHEADER(simple::header* ptr, int versionNum, const std::string& dataTypeName, const std::string&
deviceName); void testAddress();
  // private:
  double getCurrentTime();

  void fillHeader(simple::header* h, const std::string& s1, const std::string& s2);

  void fillHeaderUnique(std::unique_ptr<simple::header>& h, const std::string& s1, const std::string& s2);

  std::unique_ptr<simple::header> makeHeaderUnique(const std::string& s1, const std::string& s2);

  simple::header* makeHeader(const std::string& s1, const std::string& s2);

  std::unique_ptr<simple::generic> makeGenericUnique(simple::header& h, const std::string& s);


  simple::generic* makeGenericRaw(simple::header& h, const std::string& s);

}  // namespace simple*/
