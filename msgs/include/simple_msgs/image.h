#pragma once

#include <mutex>
#include "generic_message.h"
#include "image_generated.h"
#include "header.h"
#include "pose.h"

namespace simple_msgs
{
template <typename T>
class Image : public GenericMessage<Image<T>>
{
public:
  using GenericMessage::GenericMessage;
  /**
   * @brief Wrapper for image data. Instance type has to match the image data type.
   * @param encoding
   * @param resX Number of voxels in the x direction
   * @param resY Number of voxels in the y direction
   * @param resZ Number of voxels in the z direction
   * @param width Size of the image in the X direction
   * @param height Size of the image in the Y direction
   * @param depth Size of the image in the Z direction
   * @param data pointer to the beginning of the linearized image
   * @param dataLength size of the image buffer
   * @param headerBufPtr Pointer to the flatbuffer created for the header of the message.
   * @param originBufPtr Pointer to the flatbuffer created for the origin of the image. Origin is of type Pose.
   */
  Image<T>(const std::string& encoding, const int resX, const int resY, const int resZ, const double width,
           const double height, const double depth, T* data, int dataLength, uint8_t* headerBufPtr,
           uint8_t* originBufPtr)
    : encoding_(encoding)
    , resX_(resX)
    , resY_(resY)
    , resZ_(resZ)
    , width_(width)
    , height_(height)
    , depth_(depth)
    , header_(headerBufPtr)
    , origin_(originBufPtr)
    , data_(data)
    , dataLength_(dataLength)
    , field_mofified_(true)
  {
  }
  /**
   * @brief Construct Image wrapper from a buffer of type ImageFbs
   * @param bufferPointer pointer to the start of the buffer of type ImageFbs
   */
  Image<T>(const uint8_t* bufferPointer)
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

    // get the HeaderFbs from the bytes vector inside the image message and set each field of header to the correct
    // value
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
      case simple_msgs::data_dataUInt8:
        // get the data vector
        data_ = static_cast<const dataUInt8*>(i->imgData())->img()->data();
        break;
      case simple_msgs::data_dataInt16:
        data_ = static_cast<const dataInt16*>(i->imgData())->img()->data();
        break;
      case simple_msgs::data_dataFloat:
        data_ = static_cast<const dataFloat*>(i->imgData())->img()->data();
        break;
      case simple_msgs::data_dataDouble:
        data_ = static_cast<const dataDouble*>(i->imgData())->img()->data();
        break;
    }
    field_mofified_ = true;
    topic_ = simple_msgs::ImageFbsIdentifier();
  }
  /**
   * @brief If there are changes to the data, clears the flatbuffer builder and creates a new ImageFbs table offset. If
   * not, return the pointer to the buffer already existing
   * @return pointer to the start of the buffer of type ImageFbs
   */
  uint8_t* getBufferData() const
  {
    std::lock_guard<std::mutex> lock(mutex_);
    if (field_mofified_)
    {
      builder_->Clear();
      // flatbuffer strings and vectors must be created before the start of the table builder
      auto encodingStr = builder_->CreateString(encoding_);
      auto headerVec = builder_->CreateVector(header_.getBufferData(), header_.getBufferSize());
      auto originVec = builder_->CreateVector(origin_.getBufferData(), origin_.getBufferSize());
      auto type = getDataUnionType();
      auto elem = getDataUnionElem();
      simple_msgs::ImageFbsBuilder iBuilder(*builder_);
      // add the information
      iBuilder.add_depth(depth_);
      iBuilder.add_enconding(encodingStr);
      iBuilder.add_Header(headerVec);
      iBuilder.add_height(height_);
      iBuilder.add_imgData(elem);
      iBuilder.add_imgData_type(type);
      iBuilder.add_origin(originVec);
      iBuilder.add_resX(resX_);
      iBuilder.add_resY(resY_);
      iBuilder.add_resZ(resZ_);
      iBuilder.add_width(width_);
      auto i = iBuilder.Finish();
      simple_msgs::FinishImageFbsBuffer(
          *builder_, i);  // we have to explicitly call this method if we want the file_identifier to be set
      field_mofified_ = false;
    }
    return builder_->GetBufferPointer();
  }
  /**
   * @brief gets the size of the flatbuffer built
   * @return size of the buffer
   */
  int getBufferSize() const
  {
    return builder_->GetSize();
  }
  /**
   * @brief Gets the resolution of the image in the directions X, Y and Z.
   * @return Order of the vector: resX, resY, resZ.
   */
  std::vector<int> getResolution() const
  {
    return std::vector<int>{ resX_, resY_, resZ_ };
  }

  /**
   * @brief Gets the dimensions of the image containing the width, height and depth
   * @return Order of the vector: width, height, depth.
   */
  std::vector<double> getDimensions() const
  {
    return std::vector<double>{ width_, height_, depth_ };
  }

  /**
   * @brief getter for the image data
   * @return pointer to the begining of the image array
   */
  T* getImgData() const
  {
    return data_;
  }

  /**
   * @brief getter for the image data size
   * @return size of the image array
   */
  int getImgBufferSize() const
  {
    return dataLength_;
  }

  /**
   * @brief getter for the header of the image
   * @return pointer to the wrapper for the header of the image
   */
  simple_msgs::Header* getHeader() const
  {
    return &header_;
  }

  /**
   * @brief getter for the origin of the image
   * @return pointer to the wrapper for the pose of the image
   */
  simple_msgs::Pose* getOrigin() const
  {
    return &origin_;
  }

  /**
   * @brief getter for the encoding type of the image
   * @return string with the type of encoding
   */
  std::string getEncoding() const
  {
    return encoding_;
  }

  /**
   * @brief setter for the encoding of the image
   * @param encoding name of the type of encoding
   */
  void setEncoding(std::string encoding)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    encoding_ = encoding;
    field_mofified_ = true;
  }

  /**
   * @brief setter for the resolution of the image in the x direction
   * @param resX number of voxels in the x direction of the image
   */
  void setResX(int resX)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    resX_ = resX;
    field_mofified_ = true;
  }

  /**
   * @brief setter for the resolution of the image in the y direction
   * @param resY number of voxels in the y direction of the image
   */
  void setResY(int resY)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    resY_ = resY;
    field_mofified_ = true;
  }

  /**
   * @brief setter for the resolution of the image in the z direction
   * @param resZ number of voxels in the z direction of the image
   */
  void setResZ(int resZ)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    resZ_ = resZ;
    field_mofified_ = true;
  }

  /**
   * @brief setter for the width of the image
   * @param width size of the image in the X direction
   */
  void setWidth(double width)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    width_ = width;
    field_mofified_ = true;
  }

  /**
   * @brief setter for the height of the image
   * @param width size of the image in the Y direction
   */
  void setHeight(double height)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    height_ = height;
    field_mofified_ = true;
  }

  /**
   * @brief setter for the depth of the image
   * @param width size of the image in the Z direction
   */
  void setDepth(double depth)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    depth_ = depth;
    field_mofified_ = true;
  }

  /**
   * @brief TODO
   * @param headerBufPtr
   */
  void setHeader(uint8_t* headerBufPtr)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    header_ = headerBufPtr;
    field_mofified_ = true;
  }

  /**
   * @brief TODO
   * @param originBufPtr
   */
  void setOrigin(uint8_t* originBufPtr)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    origin_ = originBufPtr;
    field_mofified_ = true;
  }

  /**
   * @brief sets the wrapper data to the imgData and changes the field_modified to true, so the flatbuffer builder can
   * build a new ImageFbs table. Only the data type of the Image instance will be set and the others will be set to
   * empty.
   * @param imgData Vector to the linearized image, whose indexes follow the rule resX*resY*Z + resX*Y + X.
   */
  void setData(std::vector<T> imgData)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    data_ = imgData;
    field_mofified_ = true;
  }
  static const char* derivedTopic()
  {
    return ImageFbsIdentifier();
  }

private:
  int resX_{ 0 }, resY_{ 0 }, resZ_{ 0 };
  double width_{ 0.0 }, height_{ 0.0 }, depth_{ 0.0 };
  std::string encoding_{ "" };
  T* data_{ nullptr };
  int dataLength_;
  simple_msgs::Header header_;
  simple_msgs::Pose origin_;
  mutable bool field_mofified_{ false };
  mutable std::mutex mutex_;
  simple_msgs::data getDataUnionType() const;
  flatbuffers::Offset<void> getDataUnionElem() const;
};
}  // namespace simple_msgs
