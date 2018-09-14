/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SIMPLE_MSGS_NUMERIC_TYPE_HPP
#define SIMPLE_MSGS_NUMERIC_TYPE_HPP

#include <ostream>

#include "generic_message.h"

namespace simple_msgs {

/**
 * @class NumericType numeric_type.hpp.
 * @brief A generic wrapper for Flatbuffers messages that only contain a single numeric value.
 * @tparam T A specific numeric type, e.g. int, float, double, ...
 *
 * Implements the generic functionalities that are commoon to the possible data types (int, float, double).
 */
template <typename T>
class NumericType : public GenericMessage {
public:
  NumericType() = default;

  /**
   * @brief Construct the message with the given data.
   */
  NumericType(T data) : data_{data} {}

  /**
   * @brief Constructor from the buffer data, implementation is specific to the template specialization.
   */
  NumericType(const void* data);

  /**
   * @brief Copy constructor.
   */
  NumericType(const NumericType& other) : NumericType{other.data_} {}

  /**
   * @brief Copy assignment.
   */
  NumericType& operator=(const NumericType& rhs) {
    std::lock_guard<std::mutex> lock{mutex_};
    data_ = rhs.data_;
    return *this;
  }

  /**
   * @brief Copy assignment operator that uses raw memory coming from the network.
   */
  NumericType& operator=(std::shared_ptr<void*> data);

  /**
   * @brief Move constructor.
   */
  NumericType(NumericType&& other) noexcept : data_(std::move(other.data_)) {}

  /**
   * @brief Move operator.
   */
  NumericType& operator=(NumericType&& rhs) noexcept {
    std::lock_guard<std::mutex> lock{mutex_};
    data_ = std::move(rhs.data_);
    return *this;
  }

  /**
   * @brief Returns true if lhs is equal to rhs, false otherwise.
   */
  inline bool operator==(const NumericType& rhs) const { return data_ == rhs.data_; }

  /**
   * @brief Returns true if lhs is not equal to rhs, false otherwise.
   */
  inline bool operator!=(const NumericType& rhs) const { return !(*this == rhs); }

  /**
   * @brief Returns true if lhs is less than rhs, false otherwise.
   */
  inline bool operator<(const NumericType& rhs) const { return data_ < rhs.data_; }

  /**
   * @brief Returns true if lhs is greater than rhs, false otherwise.
   */
  inline bool operator>(const NumericType& rhs) const { return rhs < *this; }

  /**
   * @brief Returns true if lhs is less than or equal to rhs, false otherwise.
   */
  inline bool operator<=(const NumericType& rhs) const { return !(*this > rhs); }

  /**
   * @brief Returns true if lhs is greater than or equal to rhs, false otherwise.
   */
  inline bool operator>=(const NumericType& rhs) const { return !(*this < rhs); }

  /**
   * @brief Prefix decrement operator.
   */
  NumericType& operator--() {
    --data_;
    return *this;
  }

  /**
   * @brief Postfix decrement operator.
   */
  const NumericType operator--(int) {
    const NumericType old(*this);
    --(*this);
    return old;
  }

  /**
   * @brief Prefix increment operator.
   */
  NumericType& operator++() {
    ++data_;
    return *this;
  }

  /**
   * @brief Postfix increment operator.
   */
  const NumericType operator++(int) {
    const NumericType old(*this);
    ++(*this);
    return old;
  }

  /**
   * @brief Addition operator.
   */
  NumericType& operator+=(const NumericType& rhs) {
    data_ += rhs.data_;
    return *this;
  }

  /**
   * @brief Addition operator.
   */
  friend NumericType operator+(NumericType lhs, const NumericType& rhs) {
    lhs += rhs;
    return lhs;
  }

  /**
   * @brief Subtraction operator.
   */
  NumericType& operator-=(const NumericType& rhs) {
    data_ -= rhs.data_;
    return *this;
  }

  /**
   * @brief Subtraction operator.
   */
  friend NumericType operator-(NumericType lhs, const NumericType& rhs) {
    lhs -= rhs;
    return lhs;
  }

  /**
   * @brief Multiplication operator.
   */
  NumericType& operator*=(const NumericType& rhs) {
    data_ *= rhs.data_;
    return *this;
  }

  /**
   * @brief Multiplication operator.
   */
  friend NumericType operator*(NumericType lhs, const NumericType& rhs) {
    lhs *= rhs;
    return lhs;
  }

  /**
   * @brief Division operator.
   */
  NumericType& operator/=(const NumericType& rhs) {
    data_ /= rhs.data_;
    return *this;
  }

  /**
   * @brief Division operator.
   */
  friend NumericType operator/(NumericType lhs, const NumericType& rhs) {
    lhs /= rhs;
    return lhs;
  }

  /**
   * @brief Builds and returns the buffer accordingly to the values currently stored.
   */
  std::shared_ptr<flatbuffers::DetachedBuffer> getBufferData() const override;

  /**
   * @brief Set the numeric value.
   */
  inline void set(T data) {
    std::lock_guard<std::mutex> lock{mutex_};
    data_ = data;
  }

  /**
   * @brief Get the numeric value.
   */
  inline T get() const { return data_; }

  /**
   * @brief Returns an identifier of the message type generated by the flatbuffers.
   */
  static inline std::string getTopic();

  /**
   * @brief Stream extraction operator.
   */
  template <typename Tn>
  friend std::ostream& operator<<(std::ostream& out, const NumericType<Tn>& obj);

private:
  T data_{0};  //! The internal data.
};

/**
 * @brief Stream extraction operator.
 */
template <typename T>
std::ostream& operator<<(std::ostream& out, const NumericType<T>& obj) {
  out << obj.data_;
  return out;
}
}  // Namespace simple_msgs.

#endif  // SIMPLE_MSGS_NUMERIC_TYPE_HPP
