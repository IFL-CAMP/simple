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

#ifndef SIMPLE_MSGS_QUATERNION_STAMPED_H
#define SIMPLE_MSGS_QUATERNION_STAMPED_H

#include <array>
#include <iostream>

#include "generated/quaternion_stamped_generated.h"
#include "generic_message.h"
#include "header.h"
#include "quaternion.h"

namespace simple_msgs {
/**
 * @brief Quaternion: wrapper class around the QuaternionFbs generated code from flatbuffers.
 */
class QuaternionStamped : public GenericMessage {
public:
  QuaternionStamped() = default;
  QuaternionStamped(const Header&, const Quaternion&);
  QuaternionStamped(Header&&, Quaternion&&);
  QuaternionStamped(const void*);
  QuaternionStamped(const QuaternionStamped&);
  QuaternionStamped(QuaternionStamped&&) noexcept;

  QuaternionStamped& operator=(const QuaternionStamped&);
  QuaternionStamped& operator=(QuaternionStamped&&) noexcept;
  QuaternionStamped& operator=(std::shared_ptr<void*>);

  inline bool operator==(const QuaternionStamped& rhs) const {
    std::lock_guard<std::mutex> lock{mutex_};
    return (quaternion_ == rhs.quaternion_ && header_ == rhs.header_);
  }
  inline bool operator!=(const QuaternionStamped& rhs) const { return !(*this == rhs); }
  friend std::ostream& operator<<(std::ostream&, const QuaternionStamped&);

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

  inline const Header& getHeader() const {
    std::lock_guard<std::mutex> lock{mutex_};
    return header_;
  }

  /**
   * @brief Returns the message Quaternion.
   * @return
   */
  inline Quaternion& getQuaternion() {
    std::lock_guard<std::mutex> lock{mutex_};
    return quaternion_;
  }

  inline const Quaternion& getQuaternion() const {
    std::lock_guard<std::mutex> lock{mutex_};
    return quaternion_;
  }

  /**
   * @brief Modifies the message Header.
   */
  void setHeader(const Header&);

  /**
   * @brief Modifies the message Quaternion.
   */
  void setQuaternion(const Quaternion&);

  /**
   * @brief Returns an identifier of the message type generated by the flatbuffers.
   */

  static inline std::string getTopic() { return QuaternionStampedFbsIdentifier(); }

private:
  mutable std::mutex mutex_{};
  Header header_{};
  Quaternion quaternion_{};
};
}  // Namespace simple_msgs.

#endif  // SIMPLE_MSGS_QUATERNION_STAMPED_H
