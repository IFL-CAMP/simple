/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SIMPLE_MSGS_IMAGE_H
#define SIMPLE_MSGS_IMAGE_H

#include <mutex>

#include "generated/image_generated.h"
#include "generic_message.hpp"
#include "header.hpp"
#include "pose.hpp"

namespace simple_msgs {
/**
 * @class Image image.hpp.
 * @brief Thread-safe wrapper for a Flatbuffers Image message.
 * @tparam T Type of the internal image data: uint8, int16, float or double.
 * It contains the data of a 2D or 3D image and its metadata:
 * - the size of the image data,
 * - the image dimensions (widht, height, depth),
 * - the number of color channels,
 * - the pixel (or voxel) spacing along the possible directions,
 * - the image encoding,
 * - a Pose message representing the image origin in space,
 * - a Header message.
 */
template <typename T>
class Image : public GenericMessage {
public:
  Image() = default;

  /**
   * @brief Copy constructor.
   */
  Image(const Image& other) : Image(other, std::lock_guard<std::mutex>(other.mutex_)) {}

  /**
   * @brief Move constructor.
   */
  Image(Image&& other) noexcept : Image(std::forward<Image>(other), std::lock_guard<std::mutex>(other.mutex_)) {}

  /**
   * @brief Copy assignment operator.
   */
  Image& operator=(const Image& rhs) {
    if (this != std::addressof(rhs)) {
      std::lock(mutex_, rhs.mutex_);
      std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
      std::lock_guard<std::mutex> other_lock{rhs.mutex_, std::adopt_lock};
      header_ = rhs.header_;
      origin_ = rhs.origin_;
      encoding_ = rhs.encoding_;
      spacing_x_ = rhs.spacing_x_;
      spacing_y_ = rhs.spacing_y_;
      spacing_z_ = rhs.spacing_z_;
      width_ = rhs.width_;
      height_ = rhs.height_;
      depth_ = rhs.depth_;
      data_size_ = rhs.data_size_;
      num_channels_ = rhs.num_channels_;
      data_ = rhs.data_;
    }
    return *this;
  }

  /**
   * @brief Move assignment operator.
   */
  Image& operator=(Image&& rhs) noexcept {
    if (this != std::addressof(rhs)) {
      std::lock(mutex_, rhs.mutex_);
      std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
      std::lock_guard<std::mutex> other_lock{rhs.mutex_, std::adopt_lock};
      header_ = std::move(rhs.header_);
      origin_ = std::move(rhs.origin_);
      encoding_ = std::move(rhs.encoding_);
      spacing_x_ = std::move(rhs.spacing_x_);
      spacing_y_ = std::move(rhs.spacing_y_);
      spacing_z_ = std::move(rhs.spacing_z_);
      width_ = std::move(rhs.width_);
      height_ = std::move(rhs.height_);
      depth_ = std::move(rhs.depth_);
      data_size_ = std::move(rhs.data_size_);
      num_channels_ = std::move(rhs.num_channels_);
      data_ = std::move(rhs.data_);
    }
    return *this;
  }

  /**
   * @brief Copy assignment operator that uses raw memory coming from the network.
   */
  Image& operator=(std::shared_ptr<void*> rhs) override;

  /**
   * @brief Returns true if lhs is equal to rhs, false otherwise.
   */
  bool operator==(const Image& rhs) const {
    std::lock_guard<std::mutex> lock{mutex_};
    bool compare = ((header_ == rhs.header_) && (origin_ == rhs.origin_) && (encoding_ == rhs.encoding_) &&
                    (spacing_x_ == rhs.spacing_x_) && (spacing_y_ == rhs.spacing_y_) &&
                    (spacing_z_ == rhs.spacing_z_) && (width_ == rhs.width_) && (height_ == rhs.height_) &&
                    (depth_ == rhs.depth_) && (data_size_ == rhs.data_size_) && (num_channels_ == rhs.num_channels_));
    // The image data is actually compared only the all the other members are equal.
    if (!data_.empty() && !rhs.data_.empty()) {
      compare = compare && (memcmp(data_.getData(), (rhs.data_.getData()), data_size_) == 0);
    }
    return compare;
  }

  /**
   * @brief Returns true if lhs is not equal to rhs, false otherwise.
   */
  bool operator!=(const Image& rhs) const { return !(*this == rhs); }

  /**
   * @brief Builds and returns the buffer accordingly to the values currently stored.
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

  /**
   * @brief Returns the pixel (or voxel) spacing.
   */
  std::array<double, 3> getSpacing() const {
    std::lock_guard<std::mutex> lock{mutex_};
    return {{spacing_x_, spacing_y_, spacing_z_}};
  }

  /**
   * @brief Returns of image dimensions (width, height and depth).
   */
  std::array<uint32_t, 3> getImageDimensions() const {
    std::lock_guard<std::mutex> lock{mutex_};
    return {{width_, height_, depth_}};
  }

  /**
   * @brief Returns the actually image raw data.
   */
  const T* getImageData() const {
    std::lock_guard<std::mutex> lock{mutex_};
    return data_.getData();
  }

  /**
   * @brief Returns the size of the image in bytes.
   */
  uint64_t getImageSize() const {
    std::lock_guard<std::mutex> lock{mutex_};
    return data_size_;
  }

  /**
   * @brief Returns the message Header.
   */
  const Header& getHeader() const {
    std::lock_guard<std::mutex> lock{mutex_};
    return header_;
  }

  /**
   * @brief Returns the message Header.
   */
  Header& getHeader() {
    std::lock_guard<std::mutex> lock{mutex_};
    return header_;
  }

  /**
   * @brief Returns a Pose representing the origin of the image in space.
   */
  const Pose& getImageOrigin() const {
    std::lock_guard<std::mutex> lock{mutex_};
    return origin_;
  }

  /**
   * @brief Returns a Pose representing the origin of the image in space.
   */
  Pose& getImageOrigin() {
    std::lock_guard<std::mutex> lock{mutex_};
    return origin_;
  }

  /**
   * @brief Returns image encoding.
   */
  std::string getImageEncoding() const {
    std::lock_guard<std::mutex> lock{mutex_};
    return encoding_;
  }

  /**
   * @brief Returns number of color channels.
   */
  uint16_t getNumChannels() const {
    std::lock_guard<std::mutex> lock{mutex_};
    return num_channels_;
  }

  /**
   * @brief Modifies the image encoding.
   */
  void setImageEncoding(const std::string& encoding) {
    std::lock_guard<std::mutex> lock{mutex_};
    encoding_ = encoding;
  }

  /**
   * @brief Modifies the pixel (or voxel) spacing.
   */
  void setImageSpacing(double spacing_x, double spacing_y, double spacing_z) {
    std::lock_guard<std::mutex> lock{mutex_};
    spacing_x_ = spacing_x;
    spacing_y_ = spacing_y;
    spacing_z_ = spacing_z;
  }

  /**
   * @brief Modifies the image dimensions (width, height and depth).
   */
  void setImageDimensions(uint32_t width, uint32_t height, uint32_t depth) {
    std::lock_guard<std::mutex> lock{mutex_};
    width_ = width;
    height_ = height;
    depth_ = depth;
  }

  /**
   * @brief Modifies the message Header.
   */
  void setHeader(const Header& header) {
    std::lock_guard<std::mutex> lock{mutex_};
    header_ = header;
  }

  /**
   * @brief Modifies the message Header.
   */
  void setHeader(Header&& header) {
    std::lock_guard<std::mutex> lock{mutex_};
    header_ = std::move(header);
  }

  /**
   * @brief Modifies the Pose representing the origin of the image in space.
   */
  void setOrigin(const Pose& origin_pose) {
    std::lock_guard<std::mutex> lock{mutex_};
    origin_ = origin_pose;
  }

  /**
   * @brief Modifies the Pose representing the origin of the image in space.
   */
  void setOrigin(Pose&& origin_pose) {
    std::lock_guard<std::mutex> lock{mutex_};
    origin_ = std::move(origin_pose);
  }

  /**
   * @brief Sets the internal image data.
   * Ownership of the data is not handled by the message object, its lifetime has to be guaranteed by the user.
   * @param [in] data: Raw pointer to the beginning of the data.
   * @param [in] data_size: Total size of the data in bytes (already contemplating the number of channels).
   * @param [in] num_channels: Number of channels in the image.
   */
  void setImageData(const T* data, uint64_t data_size, uint16_t num_channels = 1) {
    std::lock_guard<std::mutex> lock{mutex_};
    data_.setData(data);
    data_size_ = data_size;
    num_channels_ = num_channels;
  }

  /**
   * @brief Sets the internal image data.
   * @param [in] data: shared pointer to the beginning of the data.
   * @param [in] data_size: Total size of the data in bytes (already contemplating the number of channels).
   * @param [in] num_channels: Number of channels in the image.
   */
  void setImageData(std::shared_ptr<const T> data, uint64_t data_size, uint16_t num_channels = 1) {
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
  //! Thread safe copy and move constructors.
  Image(const Image& other, const std::lock_guard<std::mutex>&)
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

  Image(Image&& other, const std::lock_guard<std::mutex>&) noexcept
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

  /**
   * @brief Utility class that handles the internal image data.
   * It holds either an owning pointer or a raw pointer to the data and returns the correct one when requested.
   */
  class InternalData {
  public:
    const T* getData() const {
      if (owning_data_) { return owning_data_.get(); }
      return not_owning_data_;
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

  /**
   * @brief Adds every component of the image message from a Flatbuffer object, except for the image data itself.
   */
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

  /**
   * @brief Returns the correct data type that is used for a specific template specialization of an Image message.
   */
  simple_msgs::data getDataUnionType() const;

  /**
   * @brief Returns the image data as the correct type according to the template specialization of an Image message.
   */
  flatbuffers::Offset<void> getDataUnionElem(std::shared_ptr<flatbuffers::FlatBufferBuilder> builder) const;

  mutable std::mutex mutex_{};
  simple_msgs::Header header_{};
  simple_msgs::Pose origin_{};
  std::string encoding_{""};
  double spacing_x_{0.0}, spacing_y_{0.0}, spacing_z_{0.0};
  uint32_t width_{0}, height_{0}, depth_{0};
  uint64_t data_size_{0};
  uint16_t num_channels_{1};
  InternalData data_{};
};

}  // Namespace simple_msgs.

#endif  // SIMPLE_MSGS_IMAGE_H
