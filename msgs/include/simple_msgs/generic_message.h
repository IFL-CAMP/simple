/**
* S.I.M.P.L.E. - Smart Intra-operative Messaging Platform with Less Effort
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
