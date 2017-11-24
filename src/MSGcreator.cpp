#include "MSGcreator.h"


std::shared_ptr<flatbuffers::FlatBufferBuilder> simple::createHeader(std::string datatypename, std::string devicename, int versionnum){

	std::shared_ptr<flatbuffers::FlatBufferBuilder> fbb = std::make_shared<flatbuffers::FlatBufferBuilder>();//create flatbuffer builder (general)
	//ALWAYS build strings before the MyTableBuilder is started!
	flatbuffers::Offset<flatbuffers::String> deviceNamefbb = fbb->CreateString(devicename);//strings have to be transformed into flatbuffer offset strings
	flatbuffers::Offset<flatbuffers::String> dataTypefbb = fbb->CreateString(datatypename);
	
	simple::headerBuilder hbuilder(*fbb);//create header builder
	hbuilder.add_datatypename(dataTypefbb);//add the data to the header
	hbuilder.add_devicename(deviceNamefbb);
	hbuilder.add_timestamp(getCurrentTime());
	hbuilder.add_versionnum(versionnum);
	flatbuffers::Offset<simple::header> headerfbb = hbuilder.Finish();//both builders have to be finished
	fbb->Finish(headerfbb);

	return fbb;
}
std::shared_ptr<flatbuffers::FlatBufferBuilder> simple::createVec3(double x, double y, double z){
	std::shared_ptr<flatbuffers::FlatBufferBuilder> fbb = std::make_shared<flatbuffers::FlatBufferBuilder>();//create flatbuffer builder (general)
	simple::Vec3 position(x, y, z);//constructor for struct
	auto positionOffset = fbb->CreateStruct(position);//take the struct and make into a flatbuffer offset
	fbb->Finish(positionOffset);//finish the builder with the offset
	return fbb;
}
std::shared_ptr<flatbuffers::FlatBufferBuilder> simple::createQuaternion(double e1, double e2, double e3, double e4){
	std::shared_ptr<flatbuffers::FlatBufferBuilder> fbb = std::make_shared<flatbuffers::FlatBufferBuilder>();//create flatbuffer builder (general)
	simple::quaternion orientation(e1,e2,e3,e4);//constructor for struct
	auto orientationOffset = fbb->CreateStruct(orientation);//take the struct and make into a flatbuffer offset
	fbb->Finish(orientationOffset);//finish the builder with the offset
	return fbb;
}
std::shared_ptr<flatbuffers::FlatBufferBuilder> simple::createTRANSFORM(const flatbuffers::FlatBufferBuilder& header,const flatbuffers::FlatBufferBuilder& position,
	const flatbuffers::FlatBufferBuilder& orientation){

	std::shared_ptr<flatbuffers::FlatBufferBuilder> fbb = std::make_shared<flatbuffers::FlatBufferBuilder>();//general flatbuffer builder
	simple::transformBuilder transformb(*fbb);//specific transform builder
	//nested tables are of type ubyte so I can set the constructed buffer directly
	transformb.add_orientation(*orientation.GetBufferPointer());
	transformb.add_position(*position.GetBufferPointer());
	transformb.add_Header(*header.GetBufferPointer());
	//now finish the builders
	flatbuffers::Offset<simple::transform> transformOffset = transformb.Finish();
	fbb->Finish(transformOffset);

	return fbb;
}
/*std::shared_ptr<flatbuffers::FlatBufferBuilder> createPOSITION(flatbuffers::FlatBufferBuilder header, double px, double py, double pz,
	double e1, double e2, double e3, double e4){

}
std::shared_ptr<flatbuffers::FlatBufferBuilder> createSTATUS(flatbuffers::FlatBufferBuilder header, int code, int subcode,
	const std::string& errorName, const std::string& errorMsg){

}
std::shared_ptr<flatbuffers::FlatBufferBuilder> createCAPABILITY(flatbuffers::FlatBufferBuilder header, const
	std::vector<std::string>& msgNames){

}
std::shared_ptr<flatbuffers::FlatBufferBuilder> createGENERIC_BOOL(flatbuffers::FlatBufferBuilder header, bool data){

}
std::shared_ptr<flatbuffers::FlatBufferBuilder> createGENERIC_INT(flatbuffers::FlatBufferBuilder header, int data){

}
std::shared_ptr<flatbuffers::FlatBufferBuilder> createGENERIC_FLOAT(flatbuffers::FlatBufferBuilder header, float data){

}
std::shared_ptr<flatbuffers::FlatBufferBuilder> createGENERIC_DOUBLE(flatbuffers::FlatBufferBuilder header, double data){

}
std::shared_ptr<flatbuffers::FlatBufferBuilder> createGENERIC_STR(flatbuffers::FlatBufferBuilder header, const std::string& data){

}*/
double simple::getCurrentTime() {
	std::chrono::time_point<std::chrono::system_clock> nowTime =
		std::chrono::system_clock::now();
	std::chrono::duration<double, std::ratio<1>> duration =
		std::chrono::duration_cast<std::chrono::seconds>(
		nowTime.time_since_epoch());
	return duration.count();
}