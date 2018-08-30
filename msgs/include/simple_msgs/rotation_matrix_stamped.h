/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SIMPLE_MSGS_ROTATION_MATRIX_STAMPED_H
#define SIMPLE_MSGS_ROTATION_MATRIX_STAMPED_H

#include <array>
#include <ostream>

#include "generated/rotation_matrix_stamped_generated.h"
#include "generic_message.h"
#include "header.h"
#include "rotation_matrix.h"

namespace simple_msgs {
/**
 * @class RotationMatrixStamped rotation_matrix_stamped.h.
 * @brief Wrapper for a Flatbuffers RotationMatrixStamped message.
 * It contains a RotationMatrix and a Header message.
 */
class RotationMatrixStamped : public GenericMessage {
public:
  RotationMatrixStamped() = default;

  /**
   * @brief Construct a RotationMatrixStamped message given its Header and RotationMatrix.
   */
  RotationMatrixStamped(const Header&, const RotationMatrix&);

  /**
   * @brief Construct a RotationMatrixStamped message given its Header and RotationMatrix.
   */
  RotationMatrixStamped(Header&&, RotationMatrix&&);

  /**
   * @brief Construct a RotationMatrixStamped message using a raw memory coming from network.
   */
  RotationMatrixStamped(const void*);

  /**
   * @brief Copy constructor.
   */
  RotationMatrixStamped(const RotationMatrixStamped&);

  /**
   * @brief Move constructor.
   */
  RotationMatrixStamped(RotationMatrixStamped&&) noexcept;

  /**
   * @brief Copy assignment operator.
   */
  RotationMatrixStamped& operator=(const RotationMatrixStamped&);

  /**
   * @brief Move assignment operator.
   */
  RotationMatrixStamped& operator=(RotationMatrixStamped&&) noexcept;

  /**
   * @brief Copy assignment operator that uses raw memory coming from the network.
   */
  RotationMatrixStamped& operator=(std::shared_ptr<void*>);

  /**
   * @brief Returns true if lhs is equal to rhs, false otherwise.
   */
  inline bool operator==(const RotationMatrixStamped& rhs) const {
    std::lock_guard<std::mutex> lock{mutex_};
    return (rotation_matrix_ == rhs.rotation_matrix_ && header_ == rhs.header_);
  }

  /**
   * @brief Returns true if lhs is not equal to rhs, false otherwise.
   */
  inline bool operator!=(const RotationMatrixStamped& rhs) const { return !(*this == rhs); }

  /**
   * @brief Stream extraction operator.
   */
  friend std::ostream& operator<<(std::ostream&, const RotationMatrixStamped&);

  /**
   * @brief Builds and returns the buffer accordingly to the values currently stored.
   */
  std::shared_ptr<flatbuffers::DetachedBuffer> getBufferData() const override;

  /**
   * @brief Returns the message Header.
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
   * @brief Returns the rotation matrix.
   */
  inline RotationMatrix& getRotationMatrix() {
    std::lock_guard<std::mutex> lock{mutex_};
    return rotation_matrix_;
  }

  inline const RotationMatrix& getRotationMatrix() const {
    std::lock_guard<std::mutex> lock{mutex_};
    return rotation_matrix_;
  }

  /**
   * @brief Mofidies the header.
   */
  void setHeader(const Header&);

  /**
   * @brief Mofidies the matrix.
   */
  void setRotationMatrix(const RotationMatrix&);

  /**
   * @brief Rerturns an identifier of the message type generated by the flatbuffers.
   */
  static inline std::string getTopic() { return RotationMatrixStampedFbsIdentifier(); }

private:
  mutable std::mutex mutex_{};
  Header header_{};
  RotationMatrix rotation_matrix_{};
};
}  // Namespace simple_msgs.

#endif  // SIMPLE_MSGS_ROTATION_MATRIX_STAMPED_H
