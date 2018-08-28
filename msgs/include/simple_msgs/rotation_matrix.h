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

#ifndef SIMPLE_MSGS_ROTATION_MATRIX_H
#define SIMPLE_MSGS_ROTATION_MATRIX_H

#include <array>
#include <iostream>

#include "generated/rotation_matrix_generated.h"
#include "generic_message.h"

namespace simple_msgs {
/**
 * @class RotationMatrix rotation_matrix.h.
 * @brief Wrapper for a Flatbuffers RotationMatrix message.
 * It represents a 3x3 rotation matrix.
 */
class RotationMatrix : public GenericMessage {
public:
  RotationMatrix() = default;

  /**
   * @brief Construct a RotationMatrix message, all matrix values are set to the given parameter.
   */
  RotationMatrix(double);

  /**
   * @brief Construct a RotationMatrix message using the given parameters.
   */
  RotationMatrix(double, double, double, double, double, double, double, double, double);

  /**
   * @brief Copy construct a RotationMatrix message using the given 9D array.
   */
  RotationMatrix(const std::array<double, 9>&);

  /**
   * @brief Move construct a RotationMatrix message using the given 9D array.
   */
  RotationMatrix(std::array<double, 9>&&) noexcept;

  /**
   * @brief Construct a RotationMatrix message using a raw memory coming from network.
   */
  RotationMatrix(const void*);

  /**
   * @brief Copy constructor.
   */
  RotationMatrix(const RotationMatrix&);

  /**
   * @brief Move constructor.
   */
  RotationMatrix(RotationMatrix&&) noexcept;

  /**
   * @brief Copy assignment operator.
   */
  RotationMatrix& operator=(const RotationMatrix&);

  /**
   * @brief Move assignment operator.
   */
  RotationMatrix& operator=(RotationMatrix&&) noexcept;

  /**
   * @brief Copy assignment operator that uses raw memory coming from the network.
   */
  RotationMatrix& operator=(std::shared_ptr<void*>);

  /**
   * @brief Copy assignment from a 9D array.
   */
  RotationMatrix& operator=(const std::array<double, 9>&);

  /**
   * @brief Move assignment from a 9D array.
   */
  RotationMatrix& operator=(std::array<double, 9>&&) noexcept;

  /**
   * @brief Returns true if lhs is equal to rhs, false otherwise.
   */
  inline bool operator==(const RotationMatrix& rhs) const { return data_ == rhs.data_; }

  /**
   * @brief Returns true if lhs is not equal to rhs, false otherwise.
   */
  inline bool operator!=(const RotationMatrix& rhs) const { return !(*this == rhs); }

  /**
   * @brief Stream extraction operator.
   */
  friend std::ostream& operator<<(std::ostream&, const RotationMatrix&);

  /**
   * @brief Builds and returns the buffer accordingly to the values currently stored.
   */
  std::shared_ptr<flatbuffers::DetachedBuffer> getBufferData() const override;

  /**
   * @brief Returns the rotation matrix as a linearized array.
   */
  inline std::array<double, 9> toVector() const { return data_; }

  /**
   * @brief Returns the matrix transpose.
   */
  RotationMatrix getTranspose() const;

  /**
   * @brief Returns a row of the matrix.
   * @param [in] row_index: index of the row to return in the range [0, 2].
   */
  std::array<double, 3> getRow(int row_index) const;

  /**
   * @brief Returns a column of the matrix.
   * @param [in] column_index: index of the column to return in the range [0, 2].
   */
  std::array<double, 3> getColumn(int column_index) const;

  /**
   * @brief Mofidies a row of the matrix with the given elements.
   * @param [in] row_index: index of the row to modify in the range [0, 2].
   * @param [in] values: 3D array containing the row values.

   */
  void setRow(int row_index, const std::array<double, 3>& values);

  /**
   * @brief Mofidies a column of the matrix with the given elements.
   * @param [in] column_index: index of the column to modify in the range [0, 2].
   * @param [in] values: 3D array containing the column values.
   */
  void setColumn(int column_index, const std::array<double, 3>& values);

  /**
   * @brief Rerturns an identifier of the message type generated by the flatbuffers.
   */
  static inline std::string getTopic() { return RotationMatrixFbsIdentifier(); }

  /**
   * @brief Returns a 3x3 identity matrix.
   */
  static inline const RotationMatrix& Identity() {
    static const RotationMatrix identity{1, 0, 0, 0, 1, 0, 0, 0, 1};
    return identity;
  }

private:
  std::array<double, 9> data_{{0, 0, 0, 0, 0, 0, 0, 0, 0}};  //! Internal data.
};
}  // Namespace simple_msgs.

#endif  // SIMPLE_MSGS_ROTATION_MATRIX_H
