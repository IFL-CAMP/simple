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

#ifndef SIMPLE_MSGS_HOMO_MATRIX_STAMPED_H
#define SIMPLE_MSGS_HOMO_MATRIX_STAMPED_H

#include <array>
#include <ostream>
#include "generated/homo_matrix_stamped_generated.h"
#include "generic_message.h"
#include "header.h"
#include "homo_matrix.h"

namespace simple_msgs {
/**
 * @brief HomoMatrix: wrapper class around the HomoMatrixFbs generated code from flatbuffers.
 */
class HomoMatrixStamped : public GenericMessage {
public:
  HomoMatrixStamped() = default;
  HomoMatrixStamped(const Header&, const HomoMatrix&);
  HomoMatrixStamped(Header&&, HomoMatrix&&);
  HomoMatrixStamped(const void*);
  HomoMatrixStamped(const HomoMatrixStamped&);
  HomoMatrixStamped(HomoMatrixStamped&&) noexcept;

  HomoMatrixStamped& operator=(const HomoMatrixStamped&);
  HomoMatrixStamped& operator=(HomoMatrixStamped&&) noexcept;
  HomoMatrixStamped& operator=(std::shared_ptr<void*>);

  inline bool operator==(const HomoMatrixStamped& rhs) const {
    return (matrix_ == rhs.matrix_ && header_ == rhs.header_);
  }

  inline bool operator!=(const HomoMatrixStamped& rhs) const { return !(*this == rhs); }
  friend std::ostream& operator<<(std::ostream&, const HomoMatrixStamped&);

  /**
   * @brief Builds and returns the buffer accordingly to the values currently stored.
   * @return the buffer data.
   */
  std::shared_ptr<flatbuffers::DetachedBuffer> getBufferData() const override;

  /**
   * @brief Returns message Header.
   */
  inline Header& getHeader() { return header_; }
  inline const Header& getHeader() const { return header_; }

  /**
   * @brief Returns the HomoMatrix as an array of 3 elements.
   */
  inline HomoMatrix& getMatrix() { return matrix_; }
  inline const HomoMatrix& getMatrix() const { return matrix_; }

  /**
   * @brief Modifies the message header.
   */
  void setHeader(const Header&);
  /**
   * @brief Modifies the message HomoMatrix.
   */
  void setMatrix(const HomoMatrix&);
  /**
   * @brief Returns an identifier of the message type generated by the flatbuffers.
   */
  static inline std::string getTopic() { return HomoMatrixStampedFbsIdentifier(); }

private:
  Header header_{};
  HomoMatrix matrix_{};
};
}  // Namespace simple_msgs.

#endif  // SIMPLE_MSGS_HOMO_MATRIX_STAMPED_H
