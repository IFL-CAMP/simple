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
  GenericMessage()
    : builder_(new flatbuffers::FlatBufferBuilder(1024))
  {
  }
  virtual ~GenericMessage() = default;

  virtual uint8_t* getBufferData() const = 0;
  int getBufferSize() const
  {
    getBufferData();
    return builder_->GetSize();
  }
  
  std::shared_ptr<flatbuffers::FlatBufferBuilder>* getBuilderPointer() const {
	  auto builderPointer =  new std::shared_ptr<flatbuffers::FlatBufferBuilder>(builder_);
	  return builderPointer;
  }

  inline bool isModified() const { return modified_; }
protected:
  std::shared_ptr<flatbuffers::FlatBufferBuilder> builder_;
  mutable bool modified_{true};
  mutable std::mutex mutex_;
};
}  // Namespace simple_msgs.
