#pragma once

#include "SIMPLE.pb.h"
#include <memory>

namespace simple{

	class MSGcreader{
	public:
		std::unique_ptr<SIMPLE::TRANSFORM> readTRANSFORM(SIMPLE::HEADER* header, double px, double py, double pz, double r11, double r12, double r13, double r21, double r22, double r23, double r31, double r32, double r33);
		std::unique_ptr<SIMPLE::POSITION> readPOSITION(SIMPLE::HEADER* header, double px, double py, double pz, double qi, double qj, double qk, double qr);
		std::unique_ptr<SIMPLE::STATUS> readSTATUS(SIMPLE::HEADER* header, int code, int subcode, std::string errorName, std::string errorMsg);
		std::unique_ptr<SIMPLE::CAPABILITY> readCAPABILITY(SIMPLE::HEADER* header, std::vector<std::string> msgNames);
		std::unique_ptr<SIMPLE::GENERIC> readGENERIC_BOOL(SIMPLE::HEADER* header, bool data);
		std::unique_ptr<SIMPLE::GENERIC> readGENERIC_INT(SIMPLE::HEADER* header, int data);
		std::unique_ptr<SIMPLE::GENERIC> readGENERIC_FLOAT(SIMPLE::HEADER* header, float data);
		std::unique_ptr<SIMPLE::GENERIC> readGENERIC_DOUBLE(SIMPLE::HEADER* header, double data);
		std::unique_ptr<SIMPLE::GENERIC> readGENERIC_STR(SIMPLE::HEADER* header, std::string data);

		SIMPLE::HEADER* readHEADER(int versionNum, std::string dataTypeName, std::string deviceName, double timeStamp);
	};

}