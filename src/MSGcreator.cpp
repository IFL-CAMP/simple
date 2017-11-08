#include "MSGcreator.h"

std::unique_ptr<simple::TRANSFORM> simple::MSGcreator::createTRANSFORM(
    simple::HEADER* header, double px, double py, double pz, double r11,
    double r12, double r13, double r21, double r22, double r23, double r31,
    double r32, double r33) {
  /// Creates a message of type TRANSFORM

  std::unique_ptr<simple::TRANSFORM> transform =
      std::make_unique<simple::TRANSFORM>();

  simple::Pos* pos = new simple::Pos();
  simple::Orientation* orientation = new simple::Orientation();

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
  transform->set_allocated_position(pos);
  transform->set_allocated_header(header);

  return transform;
}
std::unique_ptr<simple::POSITION> simple::MSGcreator::createPOSITION(
    simple::HEADER* header, double px, double py, double pz, double e1,
    double e2, double e3, double e4) {
  /// Creates a message of type POSITION

  std::unique_ptr<simple::POSITION> position =
      std::make_unique<simple::POSITION>();

  simple::Pos* pos = new simple::Pos();
  simple::Quaternion* quaternion = new simple::Quaternion();

  quaternion->set_e1(e1);
  quaternion->set_e2(e2);
  quaternion->set_e3(e3);
  quaternion->set_e4(e4);

  pos->set_px(px);
  pos->set_py(py);
  pos->set_pz(pz);

  position->set_allocated_orient(quaternion);
  position->set_allocated_position(pos);
  position->set_allocated_header(header);

  return position;
}
std::unique_ptr<simple::STATUS> simple::MSGcreator::createSTATUS(
    simple::HEADER* header, int code, int subcode, std::string errorName,
    std::string errorMsg) {
  /// Creates a message of type STATUS

  std::unique_ptr<simple::STATUS> stat = std::make_unique<simple::STATUS>();

  stat->set_subcode(subcode);
  stat->set_statuscode(code);
  stat->set_errormsg(errorMsg);
  stat->set_errorname(errorName);
  stat->set_allocated_header(header);

  return stat;
}
std::unique_ptr<simple::CAPABILITY> simple::MSGcreator::createCAPABILITY(
    simple::HEADER* header, std::vector<std::string> msgNames) {
  std::unique_ptr<simple::CAPABILITY> cap =
      std::make_unique<simple::CAPABILITY>();

  for (size_t i = 0; i < msgNames.size(); i++) {
    cap->add_messagename(msgNames.at(i));
  }

  cap->set_allocated_header(header);

  return cap;
}
std::unique_ptr<simple::GENERIC> simple::MSGcreator::createGENERIC_BOOL(
    simple::HEADER* header, bool data) {
  std::unique_ptr<simple::GENERIC> gen = std::make_unique<simple::GENERIC>();

  gen->set_basicbool(data);
  gen->set_allocated_header(header);

  return gen;
}
std::unique_ptr<simple::GENERIC> simple::MSGcreator::createGENERIC_INT(
    simple::HEADER* header, int data) {
  std::unique_ptr<simple::GENERIC> gen = std::make_unique<simple::GENERIC>();

  gen->set_basicint(data);
  gen->set_allocated_header(header);

  return gen;
}
std::unique_ptr<simple::GENERIC> simple::MSGcreator::createGENERIC_FLOAT(
    simple::HEADER* header, float data) {
  std::unique_ptr<simple::GENERIC> gen = std::make_unique<simple::GENERIC>();

  gen->set_basicfloat(data);
  gen->set_allocated_header(header);

  return gen;
}
std::unique_ptr<simple::GENERIC> simple::MSGcreator::createGENERIC_DOUBLE(
    simple::HEADER* header, double data) {
  std::unique_ptr<simple::GENERIC> gen = std::make_unique<simple::GENERIC>();

  gen->set_basicdouble(data);
  gen->set_allocated_header(header);

  return gen;
}
std::unique_ptr<simple::GENERIC> simple::MSGcreator::createGENERIC_STR(
    simple::HEADER* header, std::string data) {
  std::unique_ptr<simple::GENERIC> gen = std::make_unique<simple::GENERIC>();

  gen->set_basicstring(data);
  gen->set_allocated_header(header);

  return gen;
}

simple::HEADER* simple::MSGcreator::createHEADER(int versionNum,
                                                 std::string dataTypeName,
                                                 std::string deviceName) {
  /// Creates the header of the message, including version number, type of the
  /// data, name of the transmiting device and time stamp of the message.

  simple::HEADER* header = new simple::HEADER();
  // header->set_datatypename(dataTypeName);
  // header->set_devicename(deviceName);
  // header->set_timestamp(getCurrentTime());
  // header->set_versionnumber(versionNum);
  return header;
}

double simple::MSGcreator::getCurrentTime() {
  std::chrono::time_point<std::chrono::system_clock> nowTime =
      std::chrono::system_clock::now();
  std::chrono::duration<double, std::ratio<1>> duration =
      std::chrono::duration_cast<std::chrono::seconds>(
          nowTime.time_since_epoch());
  return duration.count();
}
