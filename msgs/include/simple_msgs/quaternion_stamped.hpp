/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SIMPLE_MSGS_QUATERNION_STAMPED_H
#define SIMPLE_MSGS_QUATERNION_STAMPED_H

#include <mutex>
#include <ostream>

#include "generated/quaternion_stamped_generated.h"
#include "generic_message.hpp"
#include "header.hpp"
#include "quaternion.hpp"

namespace simple_msgs {
/**
 * @class QuaternionStamped quaternion_stamped.hpp.
 * @brief Thread-safe wrapper for a Flatbuffers QuaternionStamped message.
 * It contains a Quaternion and a Header message.
 */
class QuaternionStamped : public GenericMessage {
public:
  QuaternionStamped() = default;

  /**
   * @brief Construct a QuaternionStamped message given its Header and Quaternion.
   */
  QuaternionStamped(const Header& header, const Quaternion& quaternion);

  /**
   * @brief Construct a QuaternionStamped message given its Header and Quaternion.
   */
  QuaternionStamped(Header&& header, Quaternion&& quaternion);

  /**
   * @brief Construct a QuaternionStamped message using a raw memory coming from network.
   */
  QuaternionStamped(const void* data);

  /**
   * @brief Copy constructor.
   */
  QuaternionStamped(const QuaternionStamped& other);

  /**
   * @brief Move constructor.
   */
  QuaternionStamped(QuaternionStamped&& other) noexcept;

  /**
   * @brief Copy assignment operator.
   */
  QuaternionStamped& operator=(const QuaternionStamped& rhs);

  /**
   * @brief Move assignment operator.
   */
  QuaternionStamped& operator=(QuaternionStamped&& rhs) noexcept;

  /**
   * @brief Copy assignment operator that uses raw memory coming from the network.
   */
  QuaternionStamped& operator=(std::shared_ptr<void*> rhs) override;

  /**
   * @brief Returns true if lhs is equal to rhs, false otherwise.
   */
  inline bool operator==(const QuaternionStamped& rhs) const {
    std::lock(mutex_, rhs.mutex_);
    std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
    std::lock_guard<std::mutex> other_lock{rhs.mutex_, std::adopt_lock};
    return (quaternion_ == rhs.quaternion_ && header_ == rhs.header_);
  }

  /**
   * @brief Returns true if lhs is not equal to rhs, false otherwise.
   */
  inline bool operator!=(const QuaternionStamped& rhs) const { return !(*this == rhs); }

  /**
   * @brief Stream extraction operator.
   */
  friend std::ostream& operator<<(std::ostream& out, const QuaternionStamped& q);

  /**
   * @brief Builds and returns the buffer accordingly to the values currently stored.
   */
  std::shared_ptr<flatbuffers::DetachedBuffer> getBufferData() const override;

  /**
   * @brief Returns the message Hader.
   */
  inline Header& getHeader() {
    std::lock_guard<std::mutex> lock{mutex_};
    return header_;
  }

  /**
   * @brief Returns the message Hader.
   */
  inline const Header& getHeader() const {
    std::lock_guard<std::mutex> lock{mutex_};
    return header_;
  }

  /**
   * @brief Returns the message Quaternion.
   */
  inline Quaternion& getQuaternion() {
    std::lock_guard<std::mutex> lock{mutex_};
    return quaternion_;
  }

  /**
   * @brief Returns the message Quaternion.
   */
  inline const Quaternion& getQuaternion() const {
    std::lock_guard<std::mutex> lock{mutex_};
    return quaternion_;
  }

  /**
   * @brief Modifies the message Header.
   */
  void setHeader(const Header& header);

  /**
   * @brief Modifies the message Quaternion.
   */
  void setQuaternion(const Quaternion& quaternion);

  /**
   * @brief Returns an identifier of the message type generated by the flatbuffers.
   */

  static inline std::string getTopic() { return QuaternionStampedFbsIdentifier(); }

private:
  //! Thread safe copy and move constructors.
  QuaternionStamped(const QuaternionStamped& other, const std::lock_guard<std::mutex>&);
  QuaternionStamped(QuaternionStamped&& other, const std::lock_guard<std::mutex>&) noexcept;

  mutable std::mutex mutex_{};
  Header header_{};
  Quaternion quaternion_{};
};
}  // Namespace simple_msgs.

#endif  // SIMPLE_MSGS_QUATERNION_STAMPED_H
