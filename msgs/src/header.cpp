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

#include <utility>

#include "simple_msgs/header.h"

namespace simple_msgs {
Header::Header(int seq_n, const std::string& frame_id, long long timestamp)
  : seq_n_{seq_n}, frame_id_{frame_id}, timestamp_{timestamp} {}

Header::Header(const void* data)
  : seq_n_{GetHeaderFbs(data)->sequence_number()}
  , frame_id_{GetHeaderFbs(data)->frame_id()->c_str()}
  , timestamp_{static_cast<long long>(GetHeaderFbs(data)->timestamp())} {}

Header::Header(const Header& h) : Header{h.seq_n_, h.frame_id_, h.timestamp_} {}

Header::Header(Header&& other) noexcept
  : seq_n_{std::move(other.seq_n_)}, frame_id_(std::move(other.frame_id_)), timestamp_{std::move(other.timestamp_)} {}

Header& Header::operator=(const Header& other) {
  if (this != std::addressof(other)) {
    std::lock_guard<std::mutex> lock{mutex_};
    seq_n_ = other.seq_n_;
    frame_id_ = other.frame_id_;
    timestamp_ = other.timestamp_;
  }
  return *this;
}

Header& Header::operator=(Header&& other) noexcept {
  if (this != std::addressof(other)) {
    seq_n_ = std::move(other.seq_n_);
    frame_id_ = std::move(other.frame_id_);
    timestamp_ = std::move(other.timestamp_);
  }
  return *this;
}

Header& Header::operator=(std::shared_ptr<void*> data) {
  std::lock_guard<std::mutex> lock{mutex_};
  seq_n_ = GetHeaderFbs(*data)->sequence_number();
  frame_id_ = std::move(GetHeaderFbs(*data)->frame_id()->c_str());
  timestamp_ = static_cast<long long>(GetHeaderFbs(*data)->timestamp());
  return *this;
}

std::shared_ptr<flatbuffers::DetachedBuffer> Header::getBufferData() const {
  std::lock_guard<std::mutex> lock{mutex_};
  flatbuffers::FlatBufferBuilder builder{1024};
  auto frame_id_string = builder.CreateString(frame_id_);
  HeaderFbsBuilder tmp_builder{builder};
  tmp_builder.add_frame_id(frame_id_string);
  tmp_builder.add_sequence_number(seq_n_);
  tmp_builder.add_timestamp(timestamp_);
  FinishHeaderFbsBuffer(builder, tmp_builder.Finish());
  return std::make_shared<flatbuffers::DetachedBuffer>(builder.Release());
}

void Header::setSequenceNumber(int seq_n) {
  std::lock_guard<std::mutex> lock{mutex_};
  seq_n_ = seq_n;
}

void Header::setFrameID(const std::string& frame_id) {
  std::lock_guard<std::mutex> lock{mutex_};
  frame_id_ = frame_id;
}

void Header::setTimestamp(long long timestamp) {
  std::lock_guard<std::mutex> lock{mutex_};
  timestamp_ = timestamp;
}

/**
 * @brief Stream extraction operator.
 */
std::ostream& operator<<(std::ostream& out, const Header& h) {
  out << "Header\n \t"
      << "seq_n: " << std::to_string(h.seq_n_) << "\n \t"
      << "frame_id: " << h.frame_id_ << "\n \t"
      << "timestamp: " << std::to_string(h.timestamp_) << "\n";

  return out;
}
}  // namespace simple_msgs
