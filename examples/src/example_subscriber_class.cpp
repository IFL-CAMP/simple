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

#include <iostream>

#include "simple/subscriber.hpp"
#include "simple_msgs/pose_stamped.h"

/**
 * @brief The DataContainer class
 * This is an example of how to write a small class that asynchronously receives data from a Publisher and stores it.
 * Users can query the container asking for the current value, i.e. the last one received from the Publisher, using
 * getData().
 * The received data is stored in a struct "Data" that contains the message itself and a bool that indicates if the
 * value is "new", i.e. if it has never been obtained by the user using getData().
 */
template <typename T>
class DataContainer
{
public:
  /**
   * @brief The Data struct contains the message and a bool. The bool is set to true if the message has never been
   * "read" by user. i,e, getData has not been called since the message arrived.
   */
  struct Data
  {
    Data() = default;
    Data(bool value, const T& p)
      : is_new(value)
      , message(p)
    {
    }
    bool is_new{false};
    T message{};
  };

  DataContainer() = default;
  /**
   * @brief DataContainer ctor.
   * Initializes the iternal subscriber with the given address.
   * In this example, a class member function is used as the subscriber callback.
   * The function receives a message and updates the correspondent Data member to the new value.
   *
   * Use getData() to obtain the current value of the message stored internally.
   * Since the callback function runs on its own thread, there might be a race-condition with getData() - both access
   * the internal message data. Therefore, they share a mutex that is locked as one of the functions is called.
   *
   * @param address: The address the subscriber should connect to received data.
   * This must be in the form "tcp://xxx.xxx.xxx.xxx:yyyy", with xxx.xxx.xxx.xxx the IP address of the publisher and
   * yyyy the port that it uses.
   */
  DataContainer(std::string address)
  {
    // std::bind is used to pass the member function as the callback of the subscriber.
    address_ = address;
    data_subscriber_ =
        simple::Subscriber<T>(address, std::bind(&DataContainer::memberCallback, this, std::placeholders::_1));
  }

  DataContainer& operator=(const DataContainer& other)
  {
    data_subscriber_ =
        simple::Subscriber<T>(other.address_, std::bind(&DataContainer::memberCallback, this, std::placeholders::_1));
    return *this;
  }
  /**
   * @brief memberCallback: member function to use as callback of the subscriber.
   * @param msg: message that is received from a Publisher.
   */
  void memberCallback(const T& msg)
  {
    std::lock_guard<std::mutex> lock(data_mutex_);
    data_.is_new = true;  //< The bool inside Data is set to true, as new message has arrived.
    data_.message = msg;  //< Update the stored data with the one we just received.
  }

  /**
   * @brief getData: returns the stored data.
   * @return Data
   */
  inline Data getData() const
  {
    std::lock_guard<std::mutex> lock(data_mutex_);
    auto return_data = data_;  //< The data is preparred to be returned.
    data_.is_new = false;      //< The bool inside the data is set to false, we are returning the current message. Other
                               // calls to getData will return is_new as false if no new message has been yet recevied.
    return return_data;
  }

private:
  std::string address_;
  mutable Data data_;
  simple::Subscriber<T> data_subscriber_{};  //< The subscriber.
  mutable std::mutex data_mutex_{};          //< Used to avoid race-condition between method accessing the stored data.
};

int main()
{
  DataContainer<simple_msgs::PoseStamped> container("tcp://127.0.0.1:5555");
  std::cout << "Starting subscribing." << std::endl;
  DataContainer<simple_msgs::PoseStamped> container2;
  container2 = container;

  while (true)
  {
    auto current_data = container2.getData();  //< Obtain the most recent data from the container.
    if (current_data.is_new)                   //< Continue only if it is a new message.
    {
      // Get the sequence number of the current data and print it.
      std::cout << "Data #" << current_data.message.getHeader().getSequenceNumber() << std::endl;
    }
  }
  // Ctrl + C to kill the loop.
  return 0;
}

/**
 * Using a lambda function
 * Instead of using a member function as a callback, one could just use a lambda function.
 * E.g. lines 69-70 can be substituted with the following:
 *
 * data_subscriber_ = simple::Subscriber<T>(address, [this](const T& p) {
 *  std::lock_guard<std::mutex> lock(data_mutex_);
 *  data_.is_new = true;
 *  data_.message = msg;
 *  });
 */
