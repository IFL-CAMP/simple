/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SIMPLE_MSGS_QUATERNION_H
#define SIMPLE_MSGS_QUATERNION_H

#include <array>
#include <mutex>
#include <ostream>

#include "generated/quaternion_generated.h"
#include "generic_message.h"

namespace simple_msgs {
/**
 * @class Quaternion quaternion.h.
 * @brief Thread-safe wrapper for a Flatbuffers Quaternion message.
 */
class Quaternion : public GenericMessage {
public:
  Quaternion() = default;

  /**
   * @brief Construct a Point message using the given parameters.
   * Convention: x, y, z, w.
   */
  Quaternion(double, double, double, double);

  /**
   * @brief Copy construct a Point message using the given 4D array.
   * Convention: x, y, z, w.
   */
  Quaternion(const std::array<double, 4>&);

  /**
   * @brief Move construct a Point message using the given 4D array.
   * Convention: x, y, z, w.
   */
  Quaternion(std::array<double, 4>&&) noexcept;

  /**
   * @brief Construct a Quaternion message using a raw memory coming from network.
   */
  Quaternion(const void*);

  /**
   * @brief Copy constructor.
   */
  Quaternion(const Quaternion&);

  /**
   * @brief Move constructor.
   */
  Quaternion(Quaternion&&) noexcept;

  /**
   * @brief Copy assignment operator.
   */
  Quaternion& operator=(const Quaternion&);

  /**
   * @brief Move assignment operator.
   */
  Quaternion& operator=(Quaternion&&) noexcept;

  /**
   * @brief Copy assignment operator that uses raw memory coming from the network.
   */
  Quaternion& operator=(std::shared_ptr<void*>);

  /**
   * @brief Copy assignment from a 4D array.
   */
  Quaternion& operator=(const std::array<double, 4>&);

  /**
   * @brief Move assignment from a 4D array.
   */
  Quaternion& operator=(std::array<double, 4>&&) noexcept;

  /**
   * @brief Returns true if lhs is equal to rhs, false otherwise.
   */
  inline bool operator==(const Quaternion& rhs) const {
    std::lock_guard<std::mutex> lock{mutex_};
    return data_ == rhs.data_;
  }

  /**
   * @brief Returns true if lhs is not equal to rhs, false otherwise.
   */
  inline bool operator!=(const Quaternion& rhs) const { return !(*this == rhs); }

  /**
   * @brief Stream extraction operator.
   */
  friend std::ostream& operator<<(std::ostream&, const Quaternion&);

  /**
   * @brief Builds and returns the buffer accordingly to the values currently stored.
   */
  std::shared_ptr<flatbuffers::DetachedBuffer> getBufferData() const override;

  /**
   * @brief Returns the quaternion as an array for 4 elements.
   */
  std::array<double, 4> toVector() const {
    std::lock_guard<std::mutex> lock{mutex_};
    return data_;
  }
  /**
   * @brief Returns of the x quaternion component.
   */
  double getX() const {
    std::lock_guard<std::mutex> lock{mutex_};
    return data_[0];
  }
  /**
   * @brief Returns of the y quaternion component.
   */
  double getY() const {
    std::lock_guard<std::mutex> lock{mutex_};
    return data_[1];
  }
  /**
   * @brief Returns of the z quaternion component.
   */
  double getZ() const {
    std::lock_guard<std::mutex> lock{mutex_};
    return data_[2];
  }
  /**
   * @brief Returns of the w quaternion component.
   */
  double getW() const {
    std::lock_guard<std::mutex> lock{mutex_};
    return data_[3];
  }
  /**
   * @brief Modifies the x component of the quaternion.
   */
  void setX(double);

  /**
   * @brief Modifies the y component of the quaternion.
   */
  void setY(double);

  /**
   * @brief Modifies the z component of the quaternion.
   */
  void setZ(double);

  /**
   * @brief Modifies the w component of the quaternion.
   */
  void setW(double);

  /**
   * @brief Returns an identifier of the message type generated by the flatbuffers.
   */
  static inline std::string getTopic() { return QuaternionFbsIdentifier(); }

private:
  mutable std::mutex mutex_{};
  std::array<double, 4> data_{{0, 0, 0, 1}};
};
}  // Namespace simple_msgs.

#endif  // SIMPLE_MSGS_QUATERNION_H
