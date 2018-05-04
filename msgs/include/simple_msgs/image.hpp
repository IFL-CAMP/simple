/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy Langsch - fernanda.langsch@tum.de
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SIMPLE_MSGS_IMAGE_H
#define SIMPLE_MSGS_IMAGE_H

#include <mutex>

#include "generated/image_generated.h"
#include "generic_message.h"
#include "header.h"
#include "pose.h"

namespace simple_msgs {
template <typename T>
class Image : public GenericMessage {
public:
  Image() = default;

  Image(const uint8_t* data);

  Image(const Image& other)
    : header_(other.header_)
    , origin_(other.origin_)
    , encoding_(other.encoding_)
    , resX_(other.resX_)
    , resY_(other.resY_)
    , resZ_(other.resZ_)
    , width_(other.width_)
    , height_(other.height_)
    , depth_(other.depth_)
    , data_(other.data_)
    , data_size_(other.data_size_)
    , num_channels_(other.num_channels_) {}

  Image(Image&& other) noexcept
    : header_(std::move(other.header_))
    , origin_(std::move(other.origin_))
    , encoding_(std::move(other.encoding_))
    , resX_(std::move(other.resX_))
    , resY_(std::move(other.resY_))
    , resZ_(std::move(other.resZ_))
    , width_(std::move(other.width_))
    , height_(std::move(other.height_))
    , depth_(std::move(other.depth_))
    , data_(std::move(other.data_))
    , data_size_(std::move(other.data_size_))
    , num_channels_(other.num_channels_) {}

  Image& operator=(const Image& other) {
    if (this != std::addressof(other)) {
      std::lock_guard<std::mutex> lock(mutex_);
      header_ = other.header_;
      origin_ = other.origin_;
      encoding_ = other.encoding_;
      resX_ = other.resX_;
      resY_ = other.resY_;
      resZ_ = other.resZ_;
      width_ = other.width_;
      height_ = other.height_;
      depth_ = other.depth_;
      data_ = other.data_;
      data_size_ = other.data_size_;
      num_channels_ = other.num_channels_;
    }
    return *this;
  }
  Image& operator=(Image&& other) noexcept {
    if (this != std::addressof(other)) {
      std::lock_guard<std::mutex> lock(mutex_);
      header_ = std::move(other.header_);
      origin_ = std::move(other.origin_);
      encoding_ = std::move(other.encoding_);
      resX_ = std::move(other.resX_);
      resY_ = std::move(other.resY_);
      resZ_ = std::move(other.resZ_);
      width_ = std::move(other.width_);
      height_ = std::move(other.height_);
      depth_ = std::move(other.depth_);
      data_ = std::move(other.data_);
      data_size_ = std::move(other.data_size_);
      num_channels_ = std::move(other.num_channels_);
    }
    return *this;
  }

  Image& operator=(const uint8_t* data);

  bool operator==(const Image& rhs) const {
    bool compare =
        ((header_ == rhs.header_) && (origin_ == rhs.origin_) && (encoding_ == rhs.encoding_) && (resX_ == rhs.resX_) &&
         (resY_ == rhs.resY_) && (resZ_ == rhs.resZ_) && (width_ == rhs.width_) && (height_ == rhs.height_) &&
         (depth_ == rhs.depth_) && (data_size_ == rhs.data_size_) && (num_channels_ == rhs.num_channels_));

    if (data_ && rhs.data_) {
      return (compare && (memcmp(*data_, *(rhs.data_), data_size_) == 0));
    } else {
      return compare;
    }
  }

  bool operator!=(const Image& rhs) const { return !(*this == rhs); }
  /**
   * @brief getBufferData
   * @return
   */
  std::shared_ptr<flatbuffers::DetachedBuffer> getBufferData() const override {
    std::lock_guard<std::mutex> lock(mutex_);

    auto builder = make_unique<flatbuffers::FlatBufferBuilder>(1024);
    // auto builder = std::unique_ptr<flatbuffers::FlatBufferBuilder>(new flatbuffers::FlatBufferBuilder(1024));

    auto encoding_string = builder->CreateString(encoding_);

    auto header_data = header_.getBufferData();
    auto header_vector = builder->CreateVector(header_data->data(), header_data->size());

    auto origin_data = origin_.getBufferData();
    auto origin_vector = builder->CreateVector(origin_data->data(), origin_data->size());

    auto type = getDataUnionType();
    flatbuffers::Offset<void> elem;
    if (data_) { elem = getDataUnionElem(builder); }

    ImageFbsBuilder tmp_builder(*builder);
    // add the information
    tmp_builder.add_encoding(encoding_string);
    tmp_builder.add_header(header_vector);
    tmp_builder.add_origin(origin_vector);
    if (data_) { tmp_builder.add_image(elem); }
    tmp_builder.add_image_type(type);
    tmp_builder.add_image_size(data_size_);
    tmp_builder.add_resX(resX_);
    tmp_builder.add_resY(resY_);
    tmp_builder.add_resZ(resZ_);
    tmp_builder.add_height(height_);
    tmp_builder.add_width(width_);
    tmp_builder.add_depth(depth_);
    tmp_builder.add_num_channels(num_channels_);
    FinishImageFbsBuffer(*builder, tmp_builder.Finish());

    return std::make_shared<flatbuffers::DetachedBuffer>(builder->Release());
  }

  std::array<double, 3> getResolution() const { return std::array<double, 3>{{resX_, resY_, resZ_}}; }
  std::array<int, 3> getImageDimensions() const { return std::array<int, 3>{{width_, height_, depth_}}; }
  const T* getImageData() const { return *data_; }
  int getImageSize() const { return data_size_; }
  const Header& getHeader() const { return header_; }
  Header& getHeader() { return header_; }
  const Pose& getImageOrigin() const { return origin_; }
  Pose& getImageOrigin() { return origin_; }
  std::string getImageEncoding() const { return encoding_; }
  int getNumChannels() const { return num_channels_; }
  void setImageEncoding(const std::string& encoding) {
    std::lock_guard<std::mutex> lock(mutex_);
    encoding_ = encoding;
  }

  void setImageResolution(double resX, double resY, double resZ) {
    std::lock_guard<std::mutex> lock(mutex_);
    resX_ = resX;
    resY_ = resY;
    resZ_ = resZ;
  }

  void setImageDimensions(int width, int height, int depth) {
    std::lock_guard<std::mutex> lock(mutex_);
    width_ = width;
    height_ = height;
    depth_ = depth;
  }

  void setHeader(const Header& header) {
    std::lock_guard<std::mutex> lock(mutex_);
    header_ = header;
  }

  void setOrigin(const Pose& origin_pose) {
    std::lock_guard<std::mutex> lock(mutex_);
    origin_ = origin_pose;
  }

  /**
   * @brief
   * @param data Pointer to the beginning of the data
   * @param data_size Total length of the data (already contemplating the number of channels)
   * @param num_channels Number of channels in the image
   */
  void setImageData(const T* data, int data_size, int num_channels = 1) {
    std::lock_guard<std::mutex> lock(mutex_);
    data_ = std::make_shared<const T*>(data);
    data_size_ = data_size;
    num_channels_ = num_channels;
  }

  /**
   * @brief Returns an identifier of the message type generated by the flatbuffers.
   */
  static const char* getTopic() { return ImageFbsIdentifier(); }

private:
  void fillPartialImage(const simple_msgs::ImageFbs* imageData) {
    // Set Header.
    header_ = imageData->header()->data();
    // Set Origin.
    origin_ = imageData->origin()->data();
    // Set Image Resolution.
    resX_ = imageData->resX();
    resY_ = imageData->resY();
    resZ_ = imageData->resZ();
    // Set Image Dimensions.
    width_ = imageData->width();
    height_ = imageData->height();
    depth_ = imageData->depth();
    // Set Encoding.
    encoding_ = imageData->encoding()->c_str();
    data_size_ = imageData->image_size();
    num_channels_ = imageData->num_channels();
  }

  simple_msgs::data getDataUnionType() const;
  flatbuffers::Offset<void> getDataUnionElem(const std::unique_ptr<flatbuffers::FlatBufferBuilder>& builder) const;

  simple_msgs::Header header_;
  simple_msgs::Pose origin_;
  std::string encoding_{""};

  double resX_{0.0}, resY_{0.0}, resZ_{0.0};
  int width_{0}, height_{0}, depth_{0};
  std::shared_ptr<const T*> data_{};
  int data_size_{0};
  int num_channels_{1};
};

}  // Namespace simple_msgs.

#endif  // SIMPLE_MSGS_IMAGE_H
