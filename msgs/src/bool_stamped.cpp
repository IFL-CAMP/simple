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

#include "simple_msgs/bool_stamped.h"

namespace simple_msgs
{
BoolStamped::BoolStamped()
  : GenericMessage()
{
}

BoolStamped::BoolStamped(const Bool& data, const Header& header)
  : GenericMessage()
  , data_(data)
  , header_(header)
{
}

BoolStamped::BoolStamped(const uint8_t* data)
  : GenericMessage()
  , data_(GetBoolStampedFbs(data)->data()->data())
  , header_(GetBoolStampedFbs(data)->header()->data())
{
}

BoolStamped::BoolStamped(const BoolStamped& other)
  : BoolStamped(other.data_, other.header_)
{
}

BoolStamped::BoolStamped(BoolStamped&& other)
  : GenericMessage()
  , data_(std::move(other.data_))
  , header_(std::move(other.header_))
{
}

BoolStamped& BoolStamped::operator=(const BoolStamped& other)
{
  if (this != std::addressof(other))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    data_ = other.data_;
    header_ = other.header_;
    modified_ = true;
  }
  return *this;
}

BoolStamped& BoolStamped::operator=(BoolStamped&& other)
{
  if (this != std::addressof(other))
  {
    data_ = std::move(other.data_);
    header_ = std::move(other.header_);
    modified_ = true;
  }
  return *this;
}

BoolStamped& BoolStamped::operator=(const uint8_t* data)
{
	std::lock_guard<std::mutex> lock(mutex_);
	auto b = GetBoolStampedFbs(data);
	data_ = b->data()->data();
	header_ = b->header()->data();
	modified_ = true;

	return *this;
}

bool BoolStamped::operator==(const BoolStamped& rhs) const
{
  return (data_ == rhs.data_ && header_ == rhs.header_);
}

bool BoolStamped::operator!=(const BoolStamped& rhs) const
{
  return !(*this == rhs);
}

std::shared_ptr<flatbuffers::DetachedBuffer> BoolStamped::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (modified_)
  {
    builder_->Clear();
    auto data_vector = builder_->CreateVector(data_.getBufferData(), data_.getBufferSize());
    auto header_vector = builder_->CreateVector(header_.getBufferData(), header_.getBufferSize());
    BoolStampedFbsBuilder tmp_builder(*builder_);
    tmp_builder.add_data(data_vector);
    tmp_builder.add_header(header_vector);
    FinishBoolStampedFbsBuffer(*builder_, tmp_builder.Finish());
    modified_ = false;
  }
  auto buffer = std::shared_ptr<flatbuffers::DetachedBuffer>(new flatbuffers::DetachedBuffer(builder->Release()));    
  return buffer;
}

std::ostream& operator<<(std::ostream& out, const BoolStamped& b)
{
  out << b.header_ << b.data_;
  return out;
}
}  // namespace simple_msgs
