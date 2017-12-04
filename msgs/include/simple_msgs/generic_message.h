#pragma once

#include <stdint.h>
#include <memory>
#include <flatbuffers/flatbuffers.h>

namespace simple_msgs
{
class GenericMessageBase
{
public:
  GenericMessageBase() : builder_(std::make_unique<flatbuffers::FlatBufferBuilder>(1024))
  {
  }

  virtual ~GenericMessageBase() = default;
  virtual uint8_t* getBufferData() const = 0;
  virtual int getBufferSize() const = 0;

protected:
  std::unique_ptr<flatbuffers::FlatBufferBuilder> builder_;
};
template <class Derived>
class GenericMessage : public GenericMessageBase
{public:
  static const char* topic_;
};
template <class Derived>
const char* GenericMessage<Derived>::topic_ = Derived::derivedTopic_;
}  // namespace simple_msgs
