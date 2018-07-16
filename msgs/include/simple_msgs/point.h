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
 * @brief Point: wrapper class around the PointFbs generated code from flatbuffers.
 */
class Point : public GenericMessage {
public:
  Point() = default;
  Point(double);
  Point(double, double, double);
  Point(const std::array<double, 3>&);
  Point(std::array<double, 3>&&) noexcept;
  Point(const void*);
  Point(const Point&);
  Point(Point&&) noexcept;

  Point& operator=(const Point&);
  Point& operator=(Point&&) noexcept;
  Point& operator=(std::shared_ptr<void*>);
  Point& operator=(const std::array<double, 3>&);
  Point& operator=(std::array<double, 3>&&) noexcept;

  inline bool operator==(const Point& rhs) const { return data_ == rhs.data_; }
  inline bool operator!=(const Point& rhs) const { return !(*this == rhs); }
  friend std::ostream& operator<<(std::ostream&, const Point&);

  // Unary arithmetic operators.
  Point& operator++();
  Point operator++(int);
  Point& operator--();
  Point operator--(int);

  // Binary arithmetic operators.
  Point& operator+=(const Point&);
  friend Point operator+(Point, const Point&);
  Point& operator-=(const Point&);
  friend Point operator-(Point, const Point&);
  Point& operator*=(const Point&);
  friend Point operator*(Point, const Point&);
  Point& operator/=(const Point&);
  friend Point operator/(Point, const Point&);

  /**
   * @brief Builds and returns the buffer accordingly to the values currently stored.
   * @return the buffer data.
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
  std::array<double, 3> data_{{0, 0, 0}};
};
}  // Namespace simple_msgs.

#endif  // SIMPLE_MSGS_POINT_H
