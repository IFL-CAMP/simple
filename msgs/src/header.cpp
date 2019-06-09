/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "simple_msgs/header.hpp"
#include "simple_msgs/generated/header_generated.h"

namespace simple_msgs {
Header::Header(int sequence_number, const std::string& frame_id, long long timestamp)
  : seq_n_{sequence_number}, frame_id_{frame_id}, timestamp_{timestamp} {}

Header::Header(const void* data)
  : seq_n_{GetHeaderFbs(data)->sequence_number()}
  , frame_id_{GetHeaderFbs(data)->frame_id()->c_str()}
  , timestamp_{static_cast<long long>(GetHeaderFbs(data)->timestamp())} {}

Header::Header(const Header& other, const std::lock_guard<std::mutex>&)
  : Header{other.seq_n_, other.frame_id_, other.timestamp_} {}

Header::Header(Header&& other, const std::lock_guard<std::mutex>&) noexcept
  : seq_n_{other.seq_n_}, frame_id_(std::move(other.frame_id_)), timestamp_{other.timestamp_} {}

Header::Header(const Header& other) : Header(other, std::lock_guard<std::mutex>(other.mutex_)) {}

Header::Header(Header&& other) noexcept
  : Header(std::forward<Header>(other), std::lock_guard<std::mutex>(other.mutex_)) {}

Header& Header::operator=(const Header& rhs) {
  if (this != std::addressof(rhs)) {
    std::lock(mutex_, rhs.mutex_);
    std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
    std::lock_guard<std::mutex> other_lock{rhs.mutex_, std::adopt_lock};
    seq_n_ = rhs.seq_n_;
    frame_id_ = rhs.frame_id_;
    timestamp_ = rhs.timestamp_;
  }
  return *this;
}

Header& Header::operator=(Header&& rhs) noexcept {
  if (this != std::addressof(rhs)) {
    std::lock(mutex_, rhs.mutex_);
    std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
    std::lock_guard<std::mutex> other_lock{rhs.mutex_, std::adopt_lock};
    seq_n_ = rhs.seq_n_;
    frame_id_ = std::move(rhs.frame_id_);
    timestamp_ = rhs.timestamp_;
  }
  return *this;
}

Header& Header::operator=(std::shared_ptr<void*> rhs) {
  std::lock_guard<std::mutex> lock{mutex_};
  seq_n_ = GetHeaderFbs(*rhs)->sequence_number();
  frame_id_ = std::move(GetHeaderFbs(*rhs)->frame_id()->c_str());
  timestamp_ = static_cast<long long>(GetHeaderFbs(*rhs)->timestamp());
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

std::string Header::getTopic() { return HeaderFbsIdentifier(); }

void Header::setSequenceNumber(int sequence_number) {
  std::lock_guard<std::mutex> lock{mutex_};
  seq_n_ = sequence_number;
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
  std::lock_guard<std::mutex> lock{h.mutex_};
  out << "Header\n \t"
      << "seq_n: " << std::to_string(h.seq_n_) << "\n \t"
      << "frame_id: " << h.frame_id_ << "\n \t"
      << "timestamp: " << std::to_string(h.timestamp_) << "\n";

  return out;
}
}  // namespace simple_msgs
