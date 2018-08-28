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

#ifndef SIMPLE_MSGS_BOOL_H
#define SIMPLE_MSGS_BOOL_H

#include <ostream>

#include "generated/bool_generated.h"
#include "generic_message.h"

namespace simple_msgs {
/**
 * @class Bool bool.h.
 * @brief Wrapper for a Flatbuffers Bool message. Contains a boolean value.
 */
class Bool : public GenericMessage {
public:
  Bool() = default;

  /**
   * @brief Construct a Bool message using a raw memory coming from network.
   */
  Bool(const void*);

  /**
   * @brief Construct a Bool message using the given parameter.
   */
  Bool(bool);

  /**
   * @brief Copy constructor.
   */
  Bool(const Bool&);

  /**
   * @brief Move constructor.
   */
  Bool(Bool&&) noexcept;

  /**
   * @brief Copy assignment operator.
   */
  Bool& operator=(const Bool&);

  /**
   * @brief Move assignment operator.
   */
  Bool& operator=(Bool&&) noexcept;

  /**
   * @brief Copy assignment operator that uses raw memory coming from the network.
   */
  Bool& operator=(std::shared_ptr<void*> data);

  /**
   * @brief Returns true if lhs is equal to rhs, false otherwise.
   */
  inline bool operator==(const Bool& rhs) const { return (data_ == rhs.data_); }

  /**
   * @brief Returns true if lhs is not equal to rhs, false otherwise.
   */
  inline bool operator!=(const Bool& rhs) const { return !(*this == rhs); }

  /**
   * @brief Invert the value of the internal boolean value.
   */
  inline Bool& operator!() {
    data_ = !data_;
    return *this;
  }

  /**
   * @brief Stream extraction operator.
   */
  friend std::ostream& operator<<(std::ostream& out, const Bool& b);

  /**
   * @brief Builds and returns the built buffer accordingly to the values currently stored.
   */
  std::shared_ptr<flatbuffers::DetachedBuffer> getBufferData() const override;

  /**
   * @brief Set the boolean value.
   */
  void set(bool data);

  /**
   * @brief Get the boolean value.
   */
  inline bool get() const { return data_; }

  /**
   * @brief Returns an identifier of the message type generated by the flatbuffers.
   */
  static inline std::string getTopic() { return BoolFbsIdentifier(); }

private:
  bool data_{false};  //! Internal boolean data.
};
}  // Namespace simple_msgs.

#endif  // SIMPLE_MSGS_BOOL_H
