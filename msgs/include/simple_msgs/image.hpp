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

  Image(const Image& other)
    : header_{other.header_}
    , origin_{other.origin_}
    , encoding_{other.encoding_}
    , spacing_x_{other.spacing_x_}
    , spacing_y_{other.spacing_y_}
    , spacing_z_{other.spacing_z_}
    , width_{other.width_}
    , height_{other.height_}
    , depth_{other.depth_}
    , data_size_{other.data_size_}
    , num_channels_{other.num_channels_}
    , data_{other.data_} {}

  Image(Image&& other) noexcept
    : header_{std::move(other.header_)}
    , origin_{std::move(other.origin_)}
    , encoding_{std::move(other.encoding_)}
    , spacing_x_{std::move(other.spacing_x_)}
    , spacing_y_{std::move(other.spacing_y_)}
    , spacing_z_{std::move(other.spacing_z_)}
    , width_{std::move(other.width_)}
    , height_{std::move(other.height_)}
    , depth_{std::move(other.depth_)}
    , data_size_{std::move(other.data_size_)}
    , num_channels_{std::move(other.num_channels_)}
    , data_{std::move(other.data_)} {}

  Image& operator=(const Image& other) {
    if (this != std::addressof(other)) {
      std::lock_guard<std::mutex> lock{mutex_};
      header_ = other.header_;
      origin_ = other.origin_;
      encoding_ = other.encoding_;
      spacing_x_ = other.spacing_x_;
      spacing_y_ = other.spacing_y_;
      spacing_z_ = other.spacing_z_;
      width_ = other.width_;
      height_ = other.height_;
      depth_ = other.depth_;
      data_size_ = other.data_size_;
      num_channels_ = other.num_channels_;
      data_ = other.data_;
    }
    return *this;
  }

  Image& operator=(Image&& other) noexcept {
    if (this != std::addressof(other)) {
      std::lock_guard<std::mutex> lock{mutex_};
      header_ = std::move(other.header_);
      origin_ = std::move(other.origin_);
      encoding_ = std::move(other.encoding_);
      spacing_x_ = std::move(other.spacing_x_);
      spacing_y_ = std::move(other.spacing_y_);
      spacing_z_ = std::move(other.spacing_z_);
      width_ = std::move(other.width_);
      height_ = std::move(other.height_);
      depth_ = std::move(other.depth_);
      data_size_ = std::move(other.data_size_);
      num_channels_ = std::move(other.num_channels_);
      data_ = std::move(other.data_);
    }
    return *this;
  }

  Image& operator=(std::shared_ptr<void*> data);

  bool operator==(const Image& rhs) const {
    bool compare = ((header_ == rhs.header_) && (origin_ == rhs.origin_) && (encoding_ == rhs.encoding_) &&
                    (spacing_x_ == rhs.spacing_x_) && (spacing_y_ == rhs.spacing_y_) &&
                    (spacing_z_ == rhs.spacing_z_) && (width_ == rhs.width_) && (height_ == rhs.height_) &&
                    (depth_ == rhs.depth_) && (data_size_ == rhs.data_size_) && (num_channels_ == rhs.num_channels_));

    if (!data_.empty() && !rhs.data_.empty()) {
      compare = compare && (memcmp(data_.getData(), (rhs.data_.getData()), data_size_) == 0);
    }
    return compare;
  }

  bool operator!=(const Image& rhs) const { return !(*this == rhs); }
  /**
   * @brief getBufferData
   * @return
   */
  std::shared_ptr<flatbuffers::DetachedBuffer> getBufferData() const override {
    std::lock_guard<std::mutex> lock{mutex_};
    auto builder = std::make_shared<flatbuffers::FlatBufferBuilder>(1024);

    auto encoding_string = builder->CreateString(encoding_);

    auto header_data = header_.getBufferData();
    auto header_vector = builder->CreateVector(header_data->data(), header_data->size());

    auto origin_data = origin_.getBufferData();
    auto origin_vector = builder->CreateVector(origin_data->data(), origin_data->size());

    auto type = getDataUnionType();
    flatbuffers::Offset<void> elem{};
    if (!data_.empty()) { elem = getDataUnionElem(builder); }

    ImageFbsBuilder tmp_builder{*builder};
    // add the information
    tmp_builder.add_encoding(encoding_string);
    tmp_builder.add_header(header_vector);
    tmp_builder.add_origin(origin_vector);
    if (!data_.empty()) { tmp_builder.add_image(elem); }
    tmp_builder.add_image_type(type);
    tmp_builder.add_image_size(data_size_);
    tmp_builder.add_spacing_x(spacing_x_);
    tmp_builder.add_spacing_y(spacing_y_);
    tmp_builder.add_spacing_z(spacing_z_);
    tmp_builder.add_height(height_);
    tmp_builder.add_width(width_);
    tmp_builder.add_depth(depth_);
    tmp_builder.add_num_channels(num_channels_);
    FinishImageFbsBuffer(*builder, tmp_builder.Finish());
    return std::make_shared<flatbuffers::DetachedBuffer>(builder->Release());
  }

  std::array<double, 3> getSpacing() const { return {{spacing_x_, spacing_y_, spacing_z_}}; }
  std::array<uint32_t, 3> getImageDimensions() const { return {{width_, height_, depth_}}; }
  const T* getImageData() const { return data_.getData(); }
  int getImageSize() const { return data_size_; }
  const Header& getHeader() const { return header_; }
  Header& getHeader() { return header_; }
  const Pose& getImageOrigin() const { return origin_; }
  Pose& getImageOrigin() { return origin_; }
  std::string getImageEncoding() const { return encoding_; }
  int getNumChannels() const { return num_channels_; }

  void setImageEncoding(const std::string& encoding) {
    std::lock_guard<std::mutex> lock{mutex_};
    encoding_ = encoding;
  }

  void setImageSpacing(double spacing_x, double spacing_y, double spacing_z) {
    std::lock_guard<std::mutex> lock{mutex_};
    spacing_x_ = spacing_x;
    spacing_y_ = spacing_y;
    spacing_z_ = spacing_z;
  }

  void setImageDimensions(int width, int height, int depth) {
    std::lock_guard<std::mutex> lock{mutex_};
    width_ = width;
    height_ = height;
    depth_ = depth;
  }

  void setHeader(const Header& header) {
    std::lock_guard<std::mutex> lock{mutex_};
    header_ = header;
  }

  void setHeader(Header&& header) {
    std::lock_guard<std::mutex> lock{mutex_};
    header_ = std::move(header);
  }

  void setOrigin(const Pose& origin_pose) {
    std::lock_guard<std::mutex> lock{mutex_};
    origin_ = origin_pose;
  }

  void setOrigin(Pose&& origin_pose) {
    std::lock_guard<std::mutex> lock{mutex_};
    origin_ = std::move(origin_pose);
  }

  /**
   * @brief
   * @param data Pointer to the beginning of the data
   * @param data_size Total length of the data (already contemplating the number of channels)
   * @param num_channels Number of channels in the image
   */
  void setImageData(const T* data, int data_size, int num_channels = 1) {
    std::lock_guard<std::mutex> lock{mutex_};
    data_.setData(data);
    data_size_ = data_size;
    num_channels_ = num_channels;
  }

  void setImageData(std::shared_ptr<const T> data, int data_size, int num_channels = 1) {
    std::lock_guard<std::mutex> lock{mutex_};
    data_.setData(data);
    data_size_ = data_size;
    num_channels_ = num_channels;
  }

  /**
   * @brief Returns an identifier of the message type generated by the flatbuffers.
   */
  static inline std::string getTopic() { return ImageFbsIdentifier(); }

private:
  class InternalData {
  public:
    const T* getData() const {
      if (owning_data_) {
        return owning_data_.get();
      } else {
        return not_owning_data_;
      }
    }

    bool empty() const { return (owning_data_ == nullptr && not_owning_data_ == nullptr); }

    void setData(const T* data) {
      not_owning_data_ = data;
      owning_data_.reset();
    }

    void setData(std::shared_ptr<const T> data) {
      owning_data_ = data;
      not_owning_data_ = nullptr;
    }

  private:
    std::shared_ptr<const T> owning_data_{nullptr};
    const T* not_owning_data_{nullptr};
  };

  void fillPartialImage(const simple_msgs::ImageFbs* imageData) {
    // Set Header.
    header_ = imageData->header()->data();
    // Set Origin.
    origin_ = imageData->origin()->data();
    // Set Image Spacing.
    spacing_x_ = imageData->spacing_x();
    spacing_y_ = imageData->spacing_y();
    spacing_z_ = imageData->spacing_z();
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
  flatbuffers::Offset<void> getDataUnionElem(std::shared_ptr<flatbuffers::FlatBufferBuilder> builder) const;

  simple_msgs::Header header_{};
  simple_msgs::Pose origin_{};
  std::string encoding_{""};
  double spacing_x_{0.0}, spacing_y_{0.0}, spacing_z_{0.0};
  uint32_t width_{0}, height_{0}, depth_{0};
  uint64_t data_size_{0};
  unsigned short num_channels_{1};
  InternalData data_{};
};  // namespace simple_msgs

}  // Namespace simple_msgs.

#endif  // SIMPLE_MSGS_IMAGE_H
