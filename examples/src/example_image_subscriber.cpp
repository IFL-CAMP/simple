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
#include <csignal>
#include <condition_variable>

#include <opencv2/opencv.hpp>

#include "simple/subscriber.hpp"
#include "simple_msgs/image.pb.h"

class ImageReceiver {
public:
  ImageReceiver(const std::string& address) :
  subscriber_{address, std::bind(&ImageReceiver::imageCallback, this, std::placeholders::_1)} {}

  inline cv::Mat getImage() const
  {
    std::unique_lock<std::mutex> lock(mutex_);
    condition_variable_.wait(lock);
    return image_;
  }

private:
  void imageCallback(const simple_msgs::Image& i) {
    std::unique_lock<std::mutex> lock(mutex_);
    auto image = const_cast<char*>(i.uint8_data().data());
    const cv::Mat received_image{static_cast<int>(i.width()), static_cast<int>(i.height()),
                         CV_8UC(static_cast<int>(i.channels())), image};
    received_image.copyTo(image_);
    condition_variable_.notify_all();
  }

  const simple::Subscriber<simple_msgs::Image> subscriber_{};
  cv::Mat image_{};
  mutable std::mutex mutex_{};          //< Used to avoid race-condition between method accessing the stored data.
  mutable std::condition_variable condition_variable_{};
};

static bool run = true;
void quit (int /*unused*/) { run = false; }

int main() {
  const std::string kWindowName{"Received image"};
  const std::string kAddress{"tcp://localhost:5555"};

  // OpenCV window.
  cv::namedWindow(kWindowName, cv::WINDOW_AUTOSIZE);

  std::cout << "Creating a Subscriber for Image messages." << std::endl;
  std::cout << "Images will be displayed as soon as they are received from a Publisher." << std::endl;

  ImageReceiver receiver{kAddress};

  // Signal handlers.
  std::signal(SIGTERM, quit);
  std::signal(SIGINT, quit);
  std::signal(SIGHUP, quit);

  while(run) {
    cv::imshow(kWindowName, receiver.getImage());
    cv::waitKey(1);
  }

  std::cout << "Quitting..." << std::endl;
  cv::destroyAllWindows();
  return 0;
}
