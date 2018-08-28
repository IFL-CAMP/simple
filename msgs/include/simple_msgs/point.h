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

#ifndef SIMPLE_MSGS_POINT_H
#define SIMPLE_MSGS_POINT_H

#include <array>
#include <ostream>

#include "generated/point_generated.h"
#include "generic_message.h"

namespace simple_msgs {
/**
 * @class Point point.h.
 * @brief Wrapper for a Flatbuffers Point message.
 * It represents a 3D Point by its x, y and z coordinates.
 */
class Point : public GenericMessage {
public:
  Point() = default;

  /**
   * @brief Construct a Point message, all the point coordinates are set to the given parameter.
   */
  Point(double);

  /**
   * @brief Construct a Point message using the given parameters.
   */
  Point(double, double, double);

  /**
   * @brief Copy construct a Point message using the given 3D array.
   */
  Point(const std::array<double, 3>&);

  /**
   * @brief Move construct a Point message using the given 3D array.
   */
  Point(std::array<double, 3>&&) noexcept;

  /**
   * @brief Construct a Point message using a raw memory coming from network.
   */
  Point(const void*);

  /**
   * @brief Copy constructor.
   */
  Point(const Point&);

  /**
   * @brief Move constructor.
   */
  Point(Point&&) noexcept;

  /**
   * @brief Copy assignment operator.
   */
  Point& operator=(const Point&);

  /**
   * @brief Move assignment operator.
   */
  Point& operator=(Point&&) noexcept;

  /**
   * @brief Copy assignment operator that uses raw memory coming from the network.
   */
  Point& operator=(std::shared_ptr<void*>);

  /**
   * @brief Copy assignment from a 3D array.
   */
  Point& operator=(const std::array<double, 3>&);

  /**
   * @brief Move assignment from a 3D array.
   */
  Point& operator=(std::array<double, 3>&&) noexcept;

  /**
   * @brief Returns true if lhs is equal to rhs, false otherwise.
   */
  inline bool operator==(const Point& rhs) const { return data_ == rhs.data_; }

  /**
   * @brief Returns true if lhs is not equal to rhs, false otherwise.
   */
  inline bool operator!=(const Point& rhs) const { return !(*this == rhs); }

  /**
   * @brief Stream extraction operator.
   */
  friend std::ostream& operator<<(std::ostream&, const Point&);

  /**
   * @brief Prefix increment operator.
   */
  Point& operator++();

  /**
   * @brief Postfix increment operator.
   */
  Point operator++(int);

  /**
   * @brief Prefix decrement operator.
   */
  Point& operator--();

  /**
   * @brief Postfix decrement operator.
   */
  Point operator--(int);

  /**
   * @brief Addition operator.
   */
  Point& operator+=(const Point&);

  /**
   * @brief Addition operator.
   */
  friend Point operator+(Point, const Point&);

  /**
   * @brief Subtraction operator.
   */
  Point& operator-=(const Point&);

  /**
   * @brief Subtraction operator.
   */
  friend Point operator-(Point, const Point&);

  /**
   * @brief Multiplication operator.
   */
  Point& operator*=(const Point&);

  /**
   * @brief Multiplication operator.
   */
  friend Point operator*(Point, const Point&);

  /**
   * @brief Division operator.
   */
  Point& operator/=(const Point&);

  /**
   * @brief Division operator.
   */
  friend Point operator/(Point, const Point&);

  /**
   * @brief Builds and returns the buffer accordingly to the values currently stored.
   */
  std::shared_ptr<flatbuffers::DetachedBuffer> getBufferData() const override;

  /**
   * @brief Returns the point as an array of 3 elements.
   */
  inline std::array<double, 3> toVector() const { return data_; }

  /**
   * @brief Returns of the x point coordinate.
   */
  inline double getX() const { return data_[0]; }

  /**
   * @brief Returns of the y point coordinate.
   */
  inline double getY() const { return data_[1]; }

  /**
   * @brief Returns of the z point coordinate.
   */
  inline double getZ() const { return data_[2]; }

  /**
   * @brief Modifies the x coordinate of the point.
   */
  void setX(double);

  /**
   * @brief Modifies the y coordinate of the point.
   */
  void setY(double);

  /**
   * @brief Modifies the z coordinate of the point.
   */
  void setZ(double);

  /**
   * @brief Returns an identifier of the message type generated by the flatbuffers.
   */
  static inline std::string getTopic() { return PointFbsIdentifier(); }

private:
  std::array<double, 3> data_{{0, 0, 0}};  //! Internal array.
};
}  // Namespace simple_msgs.

#endif  // SIMPLE_MSGS_POINT_H
