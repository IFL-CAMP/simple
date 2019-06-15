/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SIMPLE_MSGS_BOOL_H
#define SIMPLE_MSGS_BOOL_H

#include <atomic>
#include <ostream>

#include "generic_message.hpp"

namespace simple_msgs {
/**
 * @class Bool bool.hpp.
 * @brief Thread-safe wrapper for a Flatbuffers Bool message. Contains a boolean value.
 */
class Bool : public GenericMessage {
public:
  Bool() = default;

  /**
   * @brief Construct a Bool message using the given parameter.
   */
  Bool(bool data);

  /**
   * @brief Copy constructor.
   */
  Bool(const Bool& other);

  /**
   * @brief Move constructor.
   */
  Bool(Bool&& other) noexcept;

  /**
   * @brief Copy assignment operator.
   */
  Bool& operator=(const Bool& rhs);

  /**
   * @brief Move assignment operator.
   */
  Bool& operator=(Bool&& rhs) noexcept;

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
   * @brief Set the boolean value.
   */
  void set(bool data) { data_.store(data); }

  /**
   * @brief Get the boolean value.
   */
  inline bool get() const { return data_.load(); }

  /**
   * @brief Returns an identifier of the message type generated by the flatbuffers.
   */
  static std::string getTopic();

protected:
  /**
   * @brief Construct a Bool message using a raw memory coming from network.
   */
  explicit Bool(const void* data);

  /**
   * @brief Copy assignment operator that uses raw memory coming from the network.
   */
  Bool& operator=(std::shared_ptr<void*> rhs) override;

  /**
   * @brief Builds and returns the built buffer accordingly to the values currently stored.
   */
  std::shared_ptr<flatbuffers::DetachedBuffer> getBufferData() const override;

private:
  std::atomic<bool> data_{false};
};
}  // Namespace simple_msgs.

#endif  // SIMPLE_MSGS_BOOL_H
