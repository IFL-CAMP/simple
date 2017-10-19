#include <iostream>
#include <fstream>
#include <string>
#include "SIMPLE.pb.h"
#include "Publisher.h"



int main(int argc, char* argv[]) {

	GOOGLE_PROTOBUF_VERIFY_VERSION;
	
	Publisher pub("tcp://*:5556");

	std::unique_ptr<SIMPLE::HEADER> header = pub.createHEADER(1, "GENERIC", "My PC", 0);

	std::unique_ptr<SIMPLE::BASEMSG> baseMSG = pub.createGENERIC_BOOL(*header, true);

	pub.publish(*baseMSG);

	return 0;
}