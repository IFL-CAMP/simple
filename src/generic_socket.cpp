/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <iostream>

// Protobuf
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <google/protobuf/message.h>

// ZeroMQ
#include <zmq.hpp>

#include "simple/generic_socket.hpp"

constexpr uint16_t kMaxEndpointSize = 50;

namespace simple {

GenericSocket::GenericSocket() {}

GenericSocket::GenericSocket(const zmq_socket_type& type, const std::string& topic) : topic_{topic} {
  initSocket(type);
}

GenericSocket::~GenericSocket() {
  closeSocket();  //! The socket is closed.
}

GenericSocket::GenericSocket(GenericSocket&& other) noexcept {
  std::lock(mutex_, other.mutex_);
  std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
  std::lock_guard<std::mutex> other_lock{other.mutex_, std::adopt_lock};
  socket_ = std::move(other.socket_);
  other.socket_ = nullptr;
  topic_ = std::move(other.topic_);
  endpoint_ = std::move(other.endpoint_);
}

GenericSocket& GenericSocket::operator=(GenericSocket&& other) noexcept {
  std::lock(mutex_, other.mutex_);
  std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
  std::lock_guard<std::mutex> other_lock{other.mutex_, std::adopt_lock};
  if (other.isSocketValid()) {
    socket_ = std::move(other.socket_);
    other.socket_ = nullptr;
    topic_ = std::move(other.topic_);
    endpoint_ = std::move(other.endpoint_);
  }
  return *this;
}

void GenericSocket::bind(const std::string& address) {
  std::lock_guard<std::mutex> lock{mutex_};
  try {
    socket_->bind(address);
  } catch (const zmq::error_t& error) {
    throw std::runtime_error("[SIMPLE Error] - Cannot bind to the address/port: " + address +
                             ". ZMQ Error: " + error.what());
  }
  setEndpoint();
}

void GenericSocket::connect(const std::string& address) {
  std::lock_guard<std::mutex> lock{mutex_};
  try {
    socket_->connect(address);
  } catch (const zmq::error_t& error) {
    throw std::runtime_error("[SIMPLE Error] - Cannot connect to the address/port: " + address +
                             ". ZMQ Error: " + error.what());
  }
  setEndpoint();
}

bool GenericSocket::sendMessage(const google::protobuf::Message& message, const size_t message_size,
                                const std::string& custom_error) const {
  // Early return if socket_ has not been created yet.
  if (socket_ == nullptr) { return false; }

  // Early return if message is empty.
  if (message_size == 0) {
    std::cerr << custom_error << "Error: attempting to send a message of size " << message_size << "." << std::endl;
    return false;
  }

  std::lock_guard<std::mutex> lock{mutex_};

  try {
    // Initialize the topic message to be sent.
    zmq::message_t topic_message{&topic_[0], topic_.size()};

    // Serialize data buffer.
    auto buffer = new char[message_size];
    google::protobuf::io::ArrayOutputStream output_stream{buffer, static_cast<int>(message_size)};
    if (!message.SerializeToZeroCopyStream(&output_stream)) { return false; }

    // Functor to call when the data transmission is over. This deletes the buffer object.
    auto free_function = [](void* /*unused*/, void* hint) {
      if (hint != nullptr) {
        auto b = static_cast<char*>(hint);
        delete b;
      }
    };

    // Initialize the message itself using the buffer data.
    zmq::message_t zmq_message{buffer, static_cast<size_t>(output_stream.ByteCount()), free_function, buffer};
    // Send the topic first and add the rest of the message after it.
    auto topic_success = socket_->send(topic_message, zmq::send_flags::sndmore);
    auto message_success = socket_->send(zmq_message, zmq::send_flags::dontwait);
    // If something wrong happened, throw zmq::error_t().
    if (!topic_success.has_value() || !message_success.has_value()) { throw zmq::error_t(); }

  } catch (const zmq::error_t& error) {
    std::cerr << custom_error << "Failed to send the message. ZMQ Error: " << error.what() << std::endl;
    return false;
  }

  // Return success.
  return true;
}

bool GenericSocket::receiveMessage(google::protobuf::Message& message, const std::string& custom_error) {
  // Early return if socket_ has not been created yet.
  if (socket_ == nullptr) { return false; }

  std::lock_guard<std::mutex> lock{mutex_};
  zmq::recv_result_t success;

  // Local variables to check if data after the topic message is available and its size.
  int data_past_topic{0};
  auto data_past_topic_size{sizeof(data_past_topic)};

  // Local ZMQ message, it is used to collect the data received from the ZMQ socket.
  // This is the pointer handling the lifetime of the received data.
  auto local_message = std::unique_ptr<zmq::message_t>(new zmq::message_t{});

  // Receive the first bytes, this should match the topic message and can be used to check if the right topic (the
  // right message type) has been received. i.e. the received topic message should match the one of the template
  // argument of this socket (stored in the topic_ member variable).
  try {
    if (!socket_->recv(*local_message)) { return false; };

    // Check if the received topic matches the right message topic.
    const std::string received_message_type = static_cast<char*>(local_message->data());
    if (received_message_type.compare(0, topic_.size(), topic_, 0, topic_.size()) != 0) {
      std::cerr << custom_error << "Received message type " << received_message_type << " while expecting " << topic_
                << "." << std::endl;
      return false;
    }

    // If all is good, check that there is more data after the topic.
    socket_->getsockopt(ZMQ_RCVMORE, &data_past_topic, &data_past_topic_size);

    if (data_past_topic == 0 || data_past_topic_size == 0) {
      std::cerr << custom_error << "No data inside message." << std::endl;
      return false;
    }

    // Receive the real message.
    success = socket_->recv(*local_message);

    // Check if any data has been received.
    if (!success || local_message->empty()) { throw zmq::error_t(); }

  } catch (const zmq::error_t& error) {
    std::cerr << custom_error << "Failed to receive a message. ZMQ Error: " << error.what() << " " << error.num()
              << std::endl;
    return false;
  }

  // At this point we are sure that the message have been correctly received, it has the right topic type and it is
  // not empty.
  google::protobuf::io::ArrayInputStream input_stream{local_message->data(), static_cast<int>(local_message->size())};
  if (!message.ParseFromZeroCopyStream(&input_stream)) { return false; }
  return success.has_value();
}

void GenericSocket::filter() {
  std::lock_guard<std::mutex> lock{mutex_};
  socket_->setsockopt(ZMQ_SUBSCRIBE, topic_.c_str(), topic_.size());
}

void GenericSocket::setTimeout(int timeout) {
  std::lock_guard<std::mutex> lock{mutex_};
  socket_->setsockopt(ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
}

void GenericSocket::setLinger(int linger) {
  std::lock_guard<std::mutex> lock{mutex_};
  socket_->setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
}

void GenericSocket::initSocket(const zmq_socket_type& type) {
  std::lock_guard<std::mutex> lock{mutex_};
  if (socket_ == nullptr) {
    socket_ = std::unique_ptr<zmq::socket_t>(new zmq::socket_t{*ContextManager::instance(), static_cast<int>(type)});
  }
}

void GenericSocket::closeSocket() {
  std::lock_guard<std::mutex> lock{mutex_};
  if (socket_ != nullptr) {
    socket_->close();
    socket_ = nullptr;
  }
}

bool GenericSocket::isSocketValid() { return static_cast<bool>(socket_ != nullptr); }

void GenericSocket::setEndpoint() {
  // Query the bound endpoint from the ZMQ API.
  char last_endpoint[kMaxEndpointSize];
  size_t size = sizeof(last_endpoint);
  socket_->getsockopt(ZMQ_LAST_ENDPOINT, &last_endpoint, &size);
  endpoint_ = last_endpoint;
}

}  // namespace simple
