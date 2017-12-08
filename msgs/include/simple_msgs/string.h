#pragma once

#include "generic_message.h"
#include "string_generated.h"

namespace simple_msgs
{
class String : public GenericMessage
{
public:
  // Constructors,
  String();
  String(const std::string&);
  String(std::string&&);
  String(const char*);
  String(const uint8_t*);
  String(const String&);
  String(String&&);

  // Copy operations.
  String& operator=(const String&);
  String& operator=(String&&) noexcept;

  // Relational operators.
  inline bool operator==(const String&) const;
  inline bool operator!=(const String&) const;

  // Binary arithmetic operators
  String& operator+=(const String&);
  friend String operator+(String, const String&);

  // Stream extraction.
  friend std::ostream& operator<<(std::ostream& out, const String& s);

  /**
   * @brief Builds and returns the buffer accordingly to the values currently stored.
   */
  uint8_t* getBufferData() const;

  /**
   * @brief Returns the string information contained in the message.
   */
  inline std::string get() const { return data_; }

  /**
   * @brief Modifies the string information contained in the message.
   */
  inline void set(const std::string& data);

  /**
   * @brief Set the content to an empty string.
   */
  constexpr void clear() { data_.clear(); }

  /**
   * @brief Rerturns an identifier of the message type generated by the flatbuffers.
   */
  static inline const char* getTopic() { return StringFbsIdentifier(); }

private:
  std::string data_{""};
};
}  // Namespace simple_msgs.
