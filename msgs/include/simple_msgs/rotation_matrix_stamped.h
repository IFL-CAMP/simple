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

#ifndef SIMPLE_MSGS_ROTATION_MATRIX_STAMPED_H
#define SIMPLE_MSGS_ROTATION_MATRIX_STAMPED_H

#include <array>
#include <iostream>
#include "generated/rotation_matrix_stamped_generated.h"
#include "generic_message.h"
#include "header.h"
#include "rotation_matrix.h"

namespace simple_msgs {
/**
 * @brief RotationMatrix: wrapper class arounf the RotationMatrixFbs generated code from flatbuffers.
 * The matrix is stored colo
 */
class RotationMatrixStamped : public GenericMessage {
public:
  RotationMatrixStamped() = default;
  RotationMatrixStamped(const Header&, const RotationMatrix&);
  RotationMatrixStamped(Header&&, RotationMatrix&&);
  RotationMatrixStamped(const void*);
  RotationMatrixStamped(const RotationMatrixStamped&);
  RotationMatrixStamped(RotationMatrixStamped&&) noexcept;

  RotationMatrixStamped& operator=(const RotationMatrixStamped&);
  RotationMatrixStamped& operator=(RotationMatrixStamped&&) noexcept;
  RotationMatrixStamped& operator=(std::shared_ptr<void*>);

  inline bool operator==(const RotationMatrixStamped& rhs) const {
    std::lock_guard<std::mutex> lock{mutex_};
    return (rotation_matrix_ == rhs.rotation_matrix_ && header_ == rhs.header_);
  }

  inline bool operator!=(const RotationMatrixStamped& rhs) const { return !(*this == rhs); }
  friend std::ostream& operator<<(std::ostream&, const RotationMatrixStamped&);

  /**
   * @brief Builds and returns the buffer accordingly to the values currently stored.
   * @return the buffer data.
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
   * @param header:
   */
  void setHeader(const Header&);

  /**
   * @brief Mofidies the matrix.
   * @param rotation_matrix:
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
