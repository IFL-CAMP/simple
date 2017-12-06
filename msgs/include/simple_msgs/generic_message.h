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
  int getBufferSize() const
  {
    getBufferData();
    return builder_->GetSize();
  }

protected:
  GenericMessage()
    : builder_(std::make_unique<flatbuffers::FlatBufferBuilder>(1024))
  {
  }
  std::unique_ptr<flatbuffers::FlatBufferBuilder> builder_;
  mutable bool modified_{true};
  mutable std::mutex mutex_;
};
}  // Namespace simple_msgs.
