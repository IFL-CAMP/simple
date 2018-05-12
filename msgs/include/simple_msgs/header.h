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

#ifndef SIMPLE_MSGS_HEADER_H
#define SIMPLE_MSGS_HEADER_H

#include <ostream>

#include "generated/header_generated.h"
#include "generic_message.h"

namespace simple_msgs {
/**
 * @brief Header: wrapper class around the HeaderFbs generated code from flatbuffers.
 */
class Header : public GenericMessage {
public:
  Header() = default;
  Header(int, const std::string&, long long);
  Header(const uint8_t*);
  Header(const Header&);
  Header(Header&&) noexcept;

  Header& operator=(const Header&);
  Header& operator=(Header&&) noexcept;
  Header& operator=(std::shared_ptr<void*>);

  inline bool operator==(const Header& rhs) const {
    return (seq_n_ == rhs.seq_n_ && frame_id_ == rhs.frame_id_ && timestamp_ == rhs.timestamp_);
  }
  inline bool operator!=(const Header& rhs) const { return !(*this == rhs); }

  friend std::ostream& operator<<(std::ostream& out, const Header& h);

  /**
   * @brief Builds and returns the buffer accordingly to the values currently stored.
   */
  std::shared_ptr<flatbuffers::DetachedBuffer> getBufferData() const override;

  /**
   * @brief Returns the sequence number of the message.
   */
  inline int getSequenceNumber() const { return seq_n_; }
  /**
   * @brief Returns the frame id of the message.
   */
  inline std::string getFrameID() const { return frame_id_; }
  /**
   * @brief Returns the timestamp of the message.
   */
  inline long long getTimestamp() const { return timestamp_; }
  /**
   * @brief Modifies the sequence number of the message.
   */
  void setSequenceNumber(int);

  /**
   * @brief Modifies the frame id of the message.
   */
  void setFrameID(const std::string&);

  /**
   * @brief Modifies the timestamp of the message.
   * @param timestamp Seconds since the epoch, when the data in the message was generated.
   */
  void setTimestamp(long long);

  /**
   * @brief Returns an identifier of the message type generated by the flatbuffers.
   */
  static inline std::string getTopic() { return HeaderFbsIdentifier(); }

private:
  int seq_n_{0};
  std::string frame_id_{""};
  long long timestamp_{0};
};
}  // Namespace simple_msgs.

#endif  // SIMPLE_MSGS_HEADER_H
