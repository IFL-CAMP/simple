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

#include "simple_msgs/homo_matrix_stamped.h"

namespace simple_msgs {
HomoMatrixStamped::HomoMatrixStamped(const Header& header, const HomoMatrix& matrix)
  : header_{header}, matrix_{matrix} {}

HomoMatrixStamped::HomoMatrixStamped(Header&& header, HomoMatrix&& matrix)
  : header_{std::move(header)}, matrix_{std::move(matrix)} {}

HomoMatrixStamped::HomoMatrixStamped(const void* data)
  : header_{GetHomoMatrixStampedFbs(data)->header()->data()}
  , matrix_{GetHomoMatrixStampedFbs(data)->matrix()->data()} {}

HomoMatrixStamped::HomoMatrixStamped(const HomoMatrixStamped& other)
  : HomoMatrixStamped{other.header_, other.matrix_} {}

HomoMatrixStamped::HomoMatrixStamped(HomoMatrixStamped&& other) noexcept
  : HomoMatrixStamped{std::move(other.header_), std::move(other.matrix_)} {}

HomoMatrixStamped& HomoMatrixStamped::operator=(const HomoMatrixStamped& other) {
  if (this != std::addressof(other)) {
    std::lock_guard<std::mutex> lock{mutex_};
    header_ = other.header_;
    matrix_ = other.matrix_;
  }
  return *this;
}

HomoMatrixStamped& HomoMatrixStamped::operator=(HomoMatrixStamped&& other) noexcept {
  if (this != std::addressof(other)) {
    std::lock_guard<std::mutex> lock{mutex_};
    header_ = std::move(other.header_);
    matrix_ = std::move(other.matrix_);
  }
  return *this;
}

HomoMatrixStamped& HomoMatrixStamped::operator=(std::shared_ptr<void*> data) {
  std::lock_guard<std::mutex> lock{mutex_};
  auto p = GetHomoMatrixStampedFbs(*data);
  header_ = p->header()->data();
  matrix_ = p->matrix()->data();
  return *this;
}

std::shared_ptr<flatbuffers::DetachedBuffer> HomoMatrixStamped::getBufferData() const {
  std::lock_guard<std::mutex> lock{mutex_};
  flatbuffers::FlatBufferBuilder builder{1024};

  auto header_data = header_.getBufferData();
  auto header_vector = builder.CreateVector(header_data->data(), header_data->size());

  auto HomoMatrix_data = matrix_.getBufferData();
  auto HomoMatrix_vector = builder.CreateVector(HomoMatrix_data->data(), HomoMatrix_data->size());

  HomoMatrixStampedFbsBuilder tmp_builder{builder};
  tmp_builder.add_header(header_vector);
  tmp_builder.add_matrix(HomoMatrix_vector);
  FinishHomoMatrixStampedFbsBuffer(builder, tmp_builder.Finish());
  return std::make_shared<flatbuffers::DetachedBuffer>(builder.Release());
}

void HomoMatrixStamped::setHeader(const Header& h) {
  std::lock_guard<std::mutex> lock{mutex_};
  header_ = h;
}

void HomoMatrixStamped::setMatrix(const HomoMatrix& m) {
  std::lock_guard<std::mutex> lock{mutex_};
  matrix_ = m;
}

std::ostream& operator<<(std::ostream& out, const HomoMatrixStamped& p) {
  out << p.header_ << p.matrix_;
  return out;
}
}  // namespace simple_msgs
