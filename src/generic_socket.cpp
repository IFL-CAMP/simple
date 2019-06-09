/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <flatbuffers/flatbuffers.h>
#include <zmq.hpp>

#include "simple/generic_socket.hpp"

namespace simple {

GenericSocket::GenericSocket() : socket_{nullptr} {}

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

  // Query the bound endpoint from the ZMQ API.
  char last_endpoint[1024];
  size_t size = sizeof(last_endpoint);
  socket_->getsockopt(ZMQ_LAST_ENDPOINT, &last_endpoint, &size);
  endpoint_ = last_endpoint;
}

void GenericSocket::connect(const std::string& address) {
  std::lock_guard<std::mutex> lock{mutex_};
  try {
    socket_->connect(address);
  } catch (const zmq::error_t& error) {
    throw std::runtime_error("[SIMPLE Error] - Cannot connect to the address/port: " + address +
                             ". ZMQ Error: " + error.what());
  }
}

bool GenericSocket::sendMsg(const simple_msgs::GenericMessage& msg, const std::string& custom_error) const {
  // Early return if socket_ has not been created yet.
  if (socket_ == nullptr) { return false; }

  std::lock_guard<std::mutex> lock{mutex_};

  try {
    // This is ugly, but we need a void* from the const char*.
    auto topic_ptr = const_cast<void*>(static_cast<const void*>(topic_.c_str()));

    // Initialize the topic message to be sent.
    zmq::message_t topic_message{topic_ptr, topic_.size()};

    auto buffer = msg.getBufferData();

    // Create a shared_ptr to the given buffer data, this allows to avoid disposing the data to be sent before the
    // actual transmission is terminated. The zmq::socket_t::send() method will return as soon as the message has been
    // queued but there are no guaranteed that the data is yet transmitted. This pointer will keep the data alive and
    // dispose it when not necessary anymore.
    auto buffer_pointer = new std::shared_ptr<flatbuffers::DetachedBuffer>{buffer};

    // Functor to call when the data transmission is over. This deletes the buffer_pointer object.
    auto free_function = [](void* /*unused*/, void* hint) {
      if (hint != nullptr) {
        auto b = static_cast<std::shared_ptr<flatbuffers::DetachedBuffer>*>(hint);
        delete b;
      }
    };

    // Initialize the message itself using the buffer data.
    // The functor free_function is passed as the function to call when this zmq::message_t object has to be disposed.
    // This is automatically called when the data transmission is over.
    // buffer_pointer is passed as the pointer to use for the "hint" parameter in the free_function method.
    zmq::message_t message{buffer->data(), buffer->size(), free_function, buffer_pointer};

    // Send the topic first and add the rest of the message after it.
    auto topic_success = socket_->send(topic_message, ZMQ_SNDMORE);
    auto message_success = socket_->send(message, ZMQ_DONTWAIT);

    // If something wrong happened, throw zmq::error_t().
    if (topic_success == false || message_success == false) { throw zmq::error_t(); }

  } catch (const zmq::error_t& error) {
    std::cerr << custom_error << "Failed to send the message. ZMQ Error: " << error.what() << std::endl;
    return false;
  }

  // Return the number of bytes sent.
  return true;
}

bool GenericSocket::receiveMsg(simple_msgs::GenericMessage& msg, const std::string& custom_error) {
  // Early return if socket_ has not been created yet.
  if (socket_ == nullptr) { return false; }

  std::lock_guard<std::mutex> lock{mutex_};
  bool success{false};

  // Local variables to check if data after the topic message is available and its size.
  int data_past_topic{0};
  auto data_past_topic_size{sizeof(data_past_topic)};

  // Local ZMQ message, it is used to collect the data received from the ZMQ socket.
  // This is the pointer handling the lifetime of the received data.
  std::shared_ptr<zmq::message_t> local_message(std::make_shared<zmq::message_t>());

  // Receive the first bytes, this should match the topic message and can be used to check if the right topic (the
  // right message type) has been received. i.e. the received topic message should match the one of the template
  // argument of this socket (stored in the topic_ member variable).
  try {
    if (!socket_->recv(local_message.get())) { throw zmq::error_t(); };

    // Check if the received topic matches the right message topic.
    std::string received_message_type = static_cast<char*>(local_message->data());
    if (std::strncmp(received_message_type.c_str(), topic_.c_str(), std::strlen(topic_.c_str())) != 0) {
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
    success = socket_->recv(local_message.get());

    // Check if any data has been received.
    if (success == false || local_message->size() == 0) { throw zmq::error_t(); }

  } catch (const zmq::error_t& error) {
    std::cerr << custom_error << "Failed to receive the message. ZMQ Error: " << error.what() << std::endl;
    return false;
  }

  // At this point we are sure that the message habe been correctly received, it has the right topic type and it is
  // not empty.

  // Set the T msg to the data that has been just received.
  void* data_ptr = local_message->data();

  // Shared pointer to the internal data that shares ref counter with local_message. Since it is passed by value to
  // the operator= of T, the ref counter is increased and local_message will stay alive until the object T needs the
  // data.
  msg = std::shared_ptr<void*>{local_message, &data_ptr};

  return success;
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
    socket_ = std::unique_ptr<zmq::socket_t>(new zmq::socket_t(*ContextManager::instance(), static_cast<int>(type)));
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

}  // namespace simple
