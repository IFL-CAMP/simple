#include "MSGcreator.h"


std::unique_ptr<flatbuffers::FlatBufferBuilder> simple::createHeader(std::string datatypename, std::string devicename, int versionnum){

	std::unique_ptr<flatbuffers::FlatBufferBuilder> fbb = std::make_unique<flatbuffers::FlatBufferBuilder>();
	headerBuilder hbuilder(*fbb);
	flatbuffers::Offset<flatbuffers::String> deviceNamefbb = fbb->CreateString(devicename);
	flatbuffers::Offset<flatbuffers::String> dataTypefbb = fbb->CreateString(datatypename);
	hbuilder.add_datatypename(dataTypefbb);
	hbuilder.add_devicename(deviceNamefbb);
	hbuilder.add_timestamp(getCurrentTime());
	hbuilder.add_versionnum(versionnum);
	flatbuffers::Offset<simple::header> headerfbb = hbuilder.Finish();
	fbb->Finish(headerfbb);

	return fbb;
}
double simple::getCurrentTime() {
	std::chrono::time_point<std::chrono::system_clock> nowTime =
		std::chrono::system_clock::now();
	std::chrono::duration<double, std::ratio<1>> duration =
		std::chrono::duration_cast<std::chrono::seconds>(
		nowTime.time_since_epoch());
	return duration.count();
}

/*void simple::createTRANSFORM(std::shared_ptr<simple::transform> transform,
    simple::header* header, double px, double py, double pz, double r11,
    double r12, double r13, double r21, double r22, double r23, double r31,
    double r32, double r33) {
  /// Creates a message of type TRANSFORM

  simple::pos* pos = new simple::pos();
  simple::orientation* orientation = new simple::orientation();

  pos->set_px(px);
  pos->set_py(py);
  pos->set_pz(pz);

  orientation->set_r11(r11);
  orientation->set_r12(r12);
  orientation->set_r13(r13);
  orientation->set_r21(r21);
  orientation->set_r22(r22);
  orientation->set_r23(r23);
  orientation->set_r31(r31);
  orientation->set_r32(r32);
  orientation->set_r33(r33);

  transform->set_allocated_orient(orientation);
  transform->set_allocated_posit(pos);
  transform->set_allocated_head(header);

}
void simple::createPOSITION(std::shared_ptr<simple::position> position,
    simple::header* header, double px, double py, double pz, double e1,
    double e2, double e3, double e4) {
  /// Creates a message of type POSITION

  simple::pos* pos = new simple::pos();
  simple::quaternion* quaternion = new simple::quaternion();

  quaternion->set_e1(e1);
  quaternion->set_e2(e2);
  quaternion->set_e3(e3);
  quaternion->set_e4(e4);

  pos->set_px(px);
  pos->set_py(py);
  pos->set_pz(pz);

  position->set_allocated_orient(quaternion);
  position->set_allocated_posit(pos);
  position->set_allocated_head(header);

}
void simple::createSTATUS(std::shared_ptr<simple::status> stat,
    simple::header* header, int code, int subcode,const std::string& errorName,
    const std::string& errorMsg) {
  /// Creates a message of type STATUS

  stat->set_subcode(subcode);
  stat->set_statuscode(code);
  stat->set_errormsg(errorMsg);
  stat->set_errorname(errorName);
  stat->set_allocated_head(header);
}
void simple::createCAPABILITY(std::shared_ptr<simple::capability> cap,
    simple::header* header,const std::vector<std::string>& msgNames) {
  

  for (size_t i = 0; i < msgNames.size(); i++) {
    cap->add_messagename(msgNames.at(i));
  }

  cap->set_allocated_head(header);
}
void simple::createGENERIC_BOOL(std::shared_ptr<simple::generic> gen,
    simple::header* header, bool data) {

  gen->set_basicbool(data);
  gen->set_allocated_head(header);
}
void simple::createGENERIC_INT(std::shared_ptr<simple::generic> gen,
    simple::header* header, int data) {

  gen->set_basicint(data);
  gen->set_allocated_head(header);

}
void simple::createGENERIC_FLOAT(std::shared_ptr<simple::generic> gen,
    simple::header* header, float data) {

  gen->set_basicfloat(data);
  gen->set_allocated_head(header);

}
void simple::createGENERIC_DOUBLE(std::shared_ptr<simple::generic> gen,
    simple::header* header, double data) {

  gen->set_basicdouble(data);
  gen->set_allocated_head(header);
}

//std::shared_ptr<simple::generic> simple::createGenericDouble(const simple::header* header, const double data);


void simple::createGENERIC_STR(std::shared_ptr<simple::generic> gen,
    simple::header* header,const std::string& data) {

  gen->set_basicstring(data);
  gen->set_allocated_head(header);
}

void simple::createHEADER(simple::header* ptr, int versionNum,
                                                  const std::string& dataTypeName,
                                                  const std::string& deviceName) {
  /// Creates the header of the message, including version number, type of the
  /// data, name of the transmiting device and time stamp of the message.


	ptr->set_datatypename(dataTypeName);
	ptr->set_devicename(deviceName);
	ptr->set_timestamp(getCurrentTime());
	ptr->set_versionnumber(versionNum);
}


void simple::testAddress(){
	simple::header* header = new simple::header();
}

void simple::fillHeader(simple::header* h, const std::string& s1, const std::string& s2)
{
	h->set_datatypename(s1);
	h->set_devicename(s2);
}

void simple::fillHeaderUnique(std::unique_ptr<simple::header>& h, const std::string& s1, const std::string& s2)
{
	h->set_datatypename(s1);
	h->set_devicename(s2);
}

std::unique_ptr<simple::header> simple::makeHeaderUnique(const std::string& s1, const std::string& s2)
{
	auto p = std::make_unique<simple::header>();
	p->set_datatypename(s1);
	p->set_devicename(s2);
	return p;
}

simple::header* simple::makeHeader(const std::string& s1, const std::string& s2)
{
	auto p = new simple::header();
	p->set_datatypename(s1);
	p->set_devicename(s2);
	return p;
}

std::unique_ptr<simple::generic> simple::makeGenericUnique(simple::header& h, const std::string& s)
{
	auto g = std::make_unique<simple::generic>();
	//simple::header* internal_h = new simple::header();
	//internal_h->CopyFrom(h);
	g->set_basicstring(s);
	g->set_allocated_head(&h);
	return g;
}
simple::generic* simple::makeGenericRaw(simple::header& h, const std::string& s)
{
	simple::generic* g = new simple::generic();
	//simple::header* internal_h = new simple::header();
	//internal_h->CopyFrom(h);
	g->set_basicstring(s);
	g->set_allocated_head(&h);
	return g;
}*/