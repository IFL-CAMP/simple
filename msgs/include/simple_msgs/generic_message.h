#pragma once

#include <stdint.h>
#include <memory>
#include <flatbuffers/flatbuffers.h>

namespace simple_msgs
{
class GenericMessage
{
public:
  GenericMessage() : builder_(std::make_unique<flatbuffers::FlatBufferBuilder>(1024))
  {
  }

  virtual ~GenericMessage() = default;
  virtual uint8_t* getBufferData() const = 0;
  virtual int getBufferSize() const = 0;
  static const char* topic_;

protected:
  std::unique_ptr<flatbuffers::FlatBufferBuilder> builder_;
};
}