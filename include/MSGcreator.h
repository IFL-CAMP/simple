#pragma once

#include "SIMPLE.pb.h"
#include <memory>

namespace simple{

	class MSGcreator{
	public:
		std::unique_ptr<SIMPLE::TRANSFORM> createTRANSFORM(SIMPLE::HEADER* header, double px, double py, double pz, double r11, double r12, double r13, double r21, double r22, double r23, double r31, double r32, double r33);
		std::unique_ptr<SIMPLE::POSITION> createPOSITION(SIMPLE::HEADER* header, double px, double py, double pz, double qi, double qj, double qk, double qr);
		std::unique_ptr<SIMPLE::STATUS> createSTATUS(SIMPLE::HEADER* header, int code, int subcode, std::string errorName, std::string errorMsg);
		std::unique_ptr<SIMPLE::CAPABILITY> createCAPABILITY(SIMPLE::HEADER* header, std::vector<std::string> msgNames);
		std::unique_ptr<SIMPLE::GENERIC> createGENERIC_BOOL(SIMPLE::HEADER* header, bool data);
		std::unique_ptr<SIMPLE::GENERIC> createGENERIC_INT(SIMPLE::HEADER* header, int data);
		std::unique_ptr<SIMPLE::GENERIC> createGENERIC_FLOAT(SIMPLE::HEADER* header, float data);
		std::unique_ptr<SIMPLE::GENERIC> createGENERIC_DOUBLE(SIMPLE::HEADER* header, double data);
		std::unique_ptr<SIMPLE::GENERIC> createGENERIC_STR(SIMPLE::HEADER* header, std::string data);

		SIMPLE::HEADER* createHEADER(int versionNum, std::string dataTypeName, std::string deviceName, double timeStamp);
	};

}