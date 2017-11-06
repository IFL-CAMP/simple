#pragma once

#include "SIMPLE.pb.h"
#include <memory>

namespace simple{

	class MSGreader{
	public:
		void readTRANSFORM(std::unique_ptr<SIMPLE::TRANSFORM>, SIMPLE::HEADER* header, double px, double py, double pz, double r11, double r12, double r13, double r21, double r22, double r23, double r31, double r32, double r33);
		void readPOSITION(std::unique_ptr<SIMPLE::POSITION>, SIMPLE::HEADER* header, double px, double py, double pz, double e1, double e2, double e3, double e4);
		void readSTATUS(std::unique_ptr<SIMPLE::STATUS>, SIMPLE::HEADER* header, int code, int subcode, std::string errorName, std::string errorMsg);
		void readCAPABILITY(std::unique_ptr<SIMPLE::CAPABILITY>, SIMPLE::HEADER* header, std::vector<std::string> msgNames);
		void readGENERIC_BOOL(std::unique_ptr<SIMPLE::GENERIC>, SIMPLE::HEADER* header, bool data);
		void readGENERIC_INT(std::unique_ptr<SIMPLE::GENERIC>, SIMPLE::HEADER* header, int data);
		void readGENERIC_FLOAT(std::unique_ptr<SIMPLE::GENERIC>, SIMPLE::HEADER* header, float data);
		void readGENERIC_DOUBLE(std::unique_ptr<SIMPLE::GENERIC>, SIMPLE::HEADER* header, double data);
		void readGENERIC_STR(std::unique_ptr<SIMPLE::GENERIC>, SIMPLE::HEADER* header, std::string data);

		void readHEADER(SIMPLE::HEADER*, int versionNum, std::string dataTypeName, std::string deviceName, double timeStamp);
	};

}