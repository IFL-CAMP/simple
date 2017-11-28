#pragma once

#include <mutex>
#include "generic_message.h"
#include "image_generated.h"
#include "header.h"
#include "pose.h"

namespace simple_msgs
{
template <typename T>
class Image : public GenericMessage
{
public:
  using GenericMessage::GenericMessage;
  /**
   * @brief Wrapper for image data of type uint8.
   * @param encoding
   * @param resX
   * @param resY
   * @param resZ
   * @param width Size of the image in the X direction
   * @param height Size of the image in the Y direction
   * @param depth Size of the image in the Z direction
   * @param data Linearized vector of the image. Indexes follow the rule resX*resY*Z + resX*Y + X.
   * @param headerBufPtr Pointer to the flatbuffer created for the header of the message.
   * @param originBufPtr Pointer to the flatbuffer created for the origin of the image. Origin is of type Pose.
   */
  Image<T>(const std::string& encoding, const int resX, const int resY, const int resZ, const double width,
           const double height, const double depth, const std::vector<T>& data, uint8_t* headerBufPtr,
           uint8_t* originBufPtr)
    : encoding_(encoding)
    , resX_(resX)
    , resY_(resY)
    , resZ_(resZ)
    , width_(width)
    , height_(height)
    , depth_(depth)
    , header_(simple_msgs::Header(headerBufPtr))
    , origin_(simple_msgs::Pose(originBufPtr))
    , data_(data)
  {
  }
  /**
   * @brief TODO
   * @param bufferPointer
   */
  Image<T>(const uint8_t* bufferPointer);
  /**
   * @brief TODO
   * @return
   */
  uint8_t* getBufferData() const;
  /**
   * @brief TODO
   * @return
   */
  int getBufferSize() const;
  /**
   * @brief TODO
   * @return
   */
  std::vector<int> getResolution() const;
  /**
   * @brief TODO
   * @return
   */
  std::vector<double> getDimensions() const;
  /**
   * @brief TODO
   * @return
   */
  std::vector<T> getImgData() const;
  /**
   * @brief TODO
   * @return
   */
  simple_msgs::Header getHeader() const;
  /**
   * @brief TODO
   * @return
   */
  simple_msgs::Pose getOrigin() const;
  /**
   * @brief TODO
   * @return
   */
  std::string getEncoding() const;
  /**
   * @brief TODO
   * @param encoding
   */
  void setEncoding(std::string encoding);
  /**
   * @brief TODO
   * @param resX
   */
  void setResX(int resX);
  /**
   * @brief TODO
   * @param resY
   */
  void setResY(int resY);
  /**
   * @brief TODO
   * @param resZ
   */
  void setResZ(int resZ);
  /**
   * @brief TODO
   * @param width
   */
  void setWidth(double width);
  /**
   * @brief TODO
   * @param height
   */
  void setHeight(double height);
  /**
   * @brief TODO
   * @param depth
   */
  void setDepth(double depth);
  /**
   * @brief TODO
   * @param headerBufPtr
   */
  void setHeader(uint8_t* headerBufPtr);
  /**
   * @brief TODO
   * @param originBufPtr
   */
  void setOrigin(uint8_t* originBufPtr);
  /**
   * @brief sets the wrapper data to the imgData and changes the field_modified to true, so the flatbuffer builder can
   * build a new ImageFbs table. Only the data type of the Image instance will be set and the others will be set to
   * empty.
   * @param imgData Vector to the linearized image, whose indexes follow the rule resX*resY*Z + resX*Y + X.
   */
  void setData(std::vector<T> imgData);
private:
  int resX_{ 0 }, resY_{ 0 }, resZ_{ 0 };
  double width_{ 0.0 }, height_{ 0.0 }, depth_{ 0.0 };
  std::string encoding_{ "" };
  std::vector<T> data_{ {} };
  simple_msgs::Header header_;
  simple_msgs::Pose origin_;
  mutable bool field_mofified_{ false };
  mutable std::mutex mutex_;
};
template <typename T>
Image<T>::Image(const uint8_t* bufferPointer)
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto i = GetImageFbs(bufferPointer);

  resX_ = i->resX();
  resY_ = i->resY();
  resZ_ = i->resZ();

  width_ = i->width();
  height_ = i->height();
  depth_ = i->depth();

  encoding_ = i->enconding();

  // get the HeaderFbs from the bytes vector inside the image message and set each field of header to the correct value
  auto head = i->Header();                       // get the vector of bytes
  auto headData = head->data();                  // get the pointer to the data
  auto h = simple_msgs::GetHeaderFbs(headData);  // turn the data into a Header table
  header_.setFrameID(h->frame_id());
  header_.setSequenceNumber(h->sequence_number());
  header_.setTimestamp(h->timestamp());

  // get the PoseFbs from the bytes vector inside the image message and set each field of Pose to the correct values
  auto pose = i->origin();
  auto poseData = pose->data();
  auto p = simple_msgs::GetPoseFbs(poseData);
  origin_.setPosition(p->position()->data());
  origin_.setQuaternion(p->quaternion()->data());

  auto type = i->imgData_type();

  switch (type)
  {
    case simple_msgs::data_NONE:
      break;
    case simple_msgs::data_dataUInt8:
      // get the data vector
      auto img = static_cast<const dataUInt8*>(i->imgData());
      break;
    case simple_msgs::data_dataInt16:
      auto img = static_cast<const dataInt16*>(i->imgData());
      break;
    case simple_msgs::data_dataFloat:
      auto img = static_cast<const dataFloat*>(i->imgData());
      break;
    case simple_msgs::data_dataDouble:
      auto img = static_cast<const dataDouble*>(i->imgData());
      break;
    case simple_msgs::data_MIN:
      break;
    case simple_msgs::data_MAX:
      break;
    default:
      break;
  }
  auto imgVec = img->img();  // vector
  auto length = imgVec->size();
  data_.clear();
  // I can't believe iterating through the vector is the best option, look for a better one
  for (size_t i = 0; i < length; i++)
  {
    data_.push_back(imgVec->Get(i));
  }
  field_mofified_ = true;
}

}  // namespace simple_msgs
