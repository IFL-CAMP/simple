/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SIMPLE_MSGS_TRANSFORM_STAMPED_H
#define SIMPLE_MSGS_TRANSFORM_STAMPED_H

#include <array>
#include <ostream>

#include "header.hpp"
#include "transform.hpp"

namespace simple_msgs {
/**
 * @class TransformStamped transform_stamped.hpp.
 * @brief Thread-safe wrapper for a Flatbuffers TransformStamped message.
 * It contains a Transform and a Header message.
 */
class TransformStamped : public GenericMessage {
public:
  TransformStamped() = default;

  /**
   * @brief Construct a TransformStamped message given its Header and Transform.
   */
  TransformStamped(const Header& header, const Transform& transform);

  /**
   * @brief Construct a TransformStamped message given its Header and Transform.
   */
  TransformStamped(Header&& header, Transform&& transform);

  /**
   * @brief Copy constructor.
   */
  TransformStamped(const TransformStamped& other);

  /**
   * @brief Move constructor.
   */
  TransformStamped(TransformStamped&& other) noexcept;

  /**
   * @brief Copy assignment operator.
   */
  TransformStamped& operator=(const TransformStamped& rhs);

  /**
   * @brief Move assignment operator.
   */
  TransformStamped& operator=(TransformStamped&& rhs) noexcept;

  /**
   * @brief Returns true if lhs is equal to rhs, false otherwise.
   */
  inline bool operator==(const TransformStamped& rhs) const {
    std::lock_guard<std::mutex> lock{mutex_};
    return (transform_ == rhs.transform_ && header_ == rhs.header_);
  }

  /**
   * @brief Returns true if lhs is not equal to rhs, false otherwise.
   */
  inline bool operator!=(const TransformStamped& rhs) const { return !(*this == rhs); }

  /**
   * @brief Stream extraction operator.
   */
  friend std::ostream& operator<<(std::ostream& out, const TransformStamped& t);

  /**
   * @brief Returns the message Header.
   */
  inline Header& getHeader() {
    std::lock_guard<std::mutex> lock{mutex_};
    return header_;
  }

  /**
   * @brief Returns the message Header.
   */
  inline const Header& getHeader() const {
    std::lock_guard<std::mutex> lock{mutex_};
    return header_;
  }

  /**
   * @brief Returns the message Transform.
   */
  inline Transform& getTransform() {
    std::lock_guard<std::mutex> lock{mutex_};
    return transform_;
  }

  /**
   * @brief Returns the message Transform.
   */
  inline const Transform& getTransform() const {
    std::lock_guard<std::mutex> lock{mutex_};
    return transform_;
  }

  /**
   * @brief Modifies the message header.
   */
  void setHeader(const Header& header);

  /**
   * @brief Modifies the message Transform.
   */
  void setTransform(const Transform& transform);

  /**
   * @brief Returns an identifier of the message type generated by the flatbuffers.
   */
  static std::string getTopic();

protected:
  /**
   * @brief Construct a TransformStamped message using a raw memory coming from network.
   */
  TransformStamped(const void* data);

  /**
   * @brief Copy assignment operator that uses raw memory coming from the network.
   */
  TransformStamped& operator=(std::shared_ptr<void*> rhs) override;

  /**
   * @brief Builds and returns the buffer accordingly to the values currently stored.
   */
  std::shared_ptr<flatbuffers::DetachedBuffer> getBufferData() const override;

private:
  //! Thread safe copy and move constructors.
  TransformStamped(const TransformStamped& other, const std::lock_guard<std::mutex>&);
  TransformStamped(TransformStamped&& other, const std::lock_guard<std::mutex>&) noexcept;

  mutable std::mutex mutex_{};
  Header header_{};
  Transform transform_{};
};
}  // Namespace simple_msgs.

#endif  // SIMPLE_MSGS_TRANSFORM_STAMPED_H
