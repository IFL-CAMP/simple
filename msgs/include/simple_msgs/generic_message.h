#pragma once

#include <mutex>
#include <memory>
#include <flatbuffers/flatbuffers.h>

namespace simple_msgs
{
/**
 * @brief GenericMessageBase: base class for SIMPLE wrappers around flatbuffers messages.
 */
class GenericMessage
{
public:
  virtual ~GenericMessage() = default;
  virtual uint8_t* getBufferData() const = 0;
  virtual int getBufferSize() const = 0;

protected:
  GenericMessage()
    : builder_(std::make_unique<flatbuffers::FlatBufferBuilder>(1024))
  {
  }
  std::unique_ptr<flatbuffers::FlatBufferBuilder> builder_;
  mutable bool mofified_{true};
  mutable std::mutex mutex_;
};

/**
template <class Derived>
*/

/**
 * @brief GenericMessage: base class for SIMPLE wrappers around flatbuffers messages.
 * It contains a generic identifier that depends on the underlying message type and its implementation.
 */

/**
class GenericMessage : public GenericMessageBase
{
protected:
  using GenericMessageBase::GenericMessageBase;  //< Using the default constructor of GenericMessageBase
  static const char* topic_;
};

template <class Derived>
const char* GenericMessage<Derived>::topic_ = Derived::getTopic();
*/
}  // namespace simple_msgs
