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
   * @param headerBufSize Size of the flatbuffer created for the message
   * @param originBufPtr Pointer to the flatbuffer created for the origin of the image. Origin is of type Pose.
   * @param originBufSize Size of the flatbuffer created for the origin of the image.
   */
  Image<T>(const std::string& encoding, const int resX, const int resY, const int resZ, const double width,
        const double height, const double depth, const std::vector<T>& data, uint8_t* headerBufPtr,
        int headerBufSize, uint8_t* originBufPtr, int originBufSize)
  {
    //flatbuffers::FlatBufferBuilder fbb;
    // auto dataVec = fbb.CreateVector(data);
    // auto offset = simple_msgs::CreatedataFloat(fbb, dataVec);
  }
  /**
   * @brief TODO
   * @param bufferPointer
   */
  Image(const uint8_t* bufferPointer);
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
  std::vector<int> getResolution();
  /**
   * @brief TODO
   * @return
   */
  std::vector<double> getDimensions();
  /**
   * @brief TODO
   * @return
   */
  std::vector<T> getImgData();
  /**
   * @brief TODO
   * @return
   */

  /**
   * @brief TODO
   * @return
   */

  /**
   * @brief TODO
   * @return
   */

private:
  int resX_{ 0 }, resY_{ 0 }, resZ_{ 0 };
  double width_{ 0.0 }, height_{ 0.0 }, depth_{ 0.0 };
  std::string encoding_{ "" };
  std::vector<T> data_{ {} };
  simple_msgs::Header header_;
  simple_msgs::Pose origin_;
  //uint8_t *headerBufPtr_{ nullptr }, originBufPtr_{ nullptr };
  //int headerBufSize_{ 0 }, originBufSize_{ 0 };
  mutable bool field_mofified_{ false };
  mutable std::mutex mutex_;
};
}  // namespace simple_msgs
