#include <signal.h>
#include <fstream>
#include <iostream>
#include <string>
#include "simple_msgs/simple.pb.h"
#include "Helpers.h"



int main(int argc, char* argv[]) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  const int versionNum = 1;
  const std::string dataTypeName = "Generic";
  const std::string deviceName = "My PC";

  simple::HeaderH myHeader(versionNum, dataTypeName, deviceName);

  const int data = 5;

  simple::GenericIntH myGenInt(data, myHeader);

  int getData = myGenInt.getData();

  std::string getdeviceName = myGenInt.getDeviceName();

  std::cout << getdeviceName;


  //delete all global objects allocated by libprotobuf
  google::protobuf::ShutdownProtobufLibrary();

}
