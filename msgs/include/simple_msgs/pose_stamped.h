/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SIMPLE_MSGS_POSE_STAMPED_H
#define SIMPLE_MSGS_POSE_STAMPED_H

#include <ostream>

#include "generated/pose_stamped_generated.h"
#include "header.h"
#include "pose.h"

namespace simple_msgs {
/**
 * @class PoseStamped pose_stamped.h.
 * @brief Wrapper for a Flatbuffers PoseStamped message.
 * It contains a Pose and a Header message.
 */
class PoseStamped : public GenericMessage {
public:
  PoseStamped() = default;

  /**
   * @brief Construct a PoseStamped message given its Header and Pose.
   */
  PoseStamped(const Header&, const Pose&);

  /**
   * @brief Construct a PoseStamped message given its Header and Pose.
   */
  PoseStamped(Header&&, Pose&&);

  /**
   * @brief Construct a PoseStamped message using a raw memory coming from network.
   */
  PoseStamped(const void*);

  /**
   * @brief Copy constructor.
   */
  PoseStamped(const PoseStamped&);

  /**
   * @brief Move constructor.
   */
  PoseStamped(PoseStamped&&) noexcept;

  /**
   * @brief Copy assignment operator.
   */
  PoseStamped& operator=(const PoseStamped&);

  /**
   * @brief Move assignment operator.
   */
  PoseStamped& operator=(PoseStamped&&) noexcept;

  /**
   * @brief Copy assignment operator that uses raw memory coming from the network.
   */
  PoseStamped& operator=(std::shared_ptr<void*>);

  /**
   * @brief Returns true if lhs is equal to rhs, false otherwise.
   */
  inline bool operator==(const PoseStamped& rhs) const { return (pose_ == rhs.pose_ && header_ == rhs.header_); }

  /**
   * @brief Returns true if lhs is not equal to rhs, false otherwise.
   */
  inline bool operator!=(const PoseStamped& rhs) const { return !(*this == rhs); }

  /**
   * @brief Stream extraction operator.
   */
  friend std::ostream& operator<<(std::ostream&, const PoseStamped&);

  /**
   * @brief Builds and returns the buffer accordingly to the values currently stored.
   */
  std::shared_ptr<flatbuffers::DetachedBuffer> getBufferData() const override;

  /**
   * @brief Returns message Header.
   */
  inline Header& getHeader() { return header_; }

  /**
   * @brief Returns message Header.
   */
  inline const Header& getHeader() const { return header_; }

  /**
   * @brief Returns the message Pose.
   */
  inline Pose& getPose() { return pose_; }

  /**
   * @brief Returns the message Pose.
   */
  inline const Pose& getPose() const { return pose_; }

  /**
   * @brief Modifies the message Header.
   */
  void setHeader(const Header&);

  /**
   * @brief Modifies the message Pose.
   */
  void setPose(const Pose&);

  /**
   * @brief Returns an identifier of the message type generated by the flatbuffers.
   */
  static inline std::string getTopic() { return PoseStampedFbsIdentifier(); }

private:
  Header header_{};
  Pose pose_{};
};
}  // Namespace simple_msgs.

#endif  // SIMPLE_MSGS_POSE_STAMPED_H
