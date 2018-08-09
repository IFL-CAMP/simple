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

#include "simple_msgs/homo_matrix.h"

namespace simple_msgs {
HomoMatrix::HomoMatrix(const Point& point, const RotationMatrix& matrix) : point_{point}, matrix_{matrix} {}

HomoMatrix::HomoMatrix(Point&& point, RotationMatrix&& matrix) : point_{std::move(point)}, matrix_{std::move(matrix)} {}

HomoMatrix::HomoMatrix(const void* data)
  : point_{GetHomoMatrixFbs(data)->point()->data()}, matrix_{GetHomoMatrixFbs(data)->matrix()->data()} {}

HomoMatrix::HomoMatrix(const HomoMatrix& other) : HomoMatrix{other.point_, other.matrix_} {}

HomoMatrix::HomoMatrix(HomoMatrix&& other) noexcept : HomoMatrix{std::move(other.point_), std::move(other.matrix_)} {}

HomoMatrix& HomoMatrix::operator=(const HomoMatrix& other) {
  if (this != std::addressof(other)) {
    std::lock_guard<std::mutex> lock{mutex_};
    point_ = other.point_;
    matrix_ = other.matrix_;
  }
  return *this;
}

HomoMatrix& HomoMatrix::operator=(HomoMatrix&& other) noexcept {
  if (this != std::addressof(other)) {
    std::lock_guard<std::mutex> lock{mutex_};
    point_ = std::move(other.point_);
    matrix_ = std::move(other.matrix_);
  }
  return *this;
}

HomoMatrix& HomoMatrix::operator=(std::shared_ptr<void*> data) {
  std::lock_guard<std::mutex> lock{mutex_};
  auto p = GetHomoMatrixFbs(*data);
  point_ = p->point()->data();
  matrix_ = p->matrix()->data();
  return *this;
}

std::shared_ptr<flatbuffers::DetachedBuffer> HomoMatrix::getBufferData() const {
  std::lock_guard<std::mutex> lock{mutex_};
  flatbuffers::FlatBufferBuilder builder{1024};

  auto point_data = point_.getBufferData();
  auto point_vector = builder.CreateVector(point_data->data(), point_data->size());

  auto matrix_data = matrix_.getBufferData();
  auto matrix_vector = builder.CreateVector(matrix_data->data(), matrix_data->size());

  HomoMatrixFbsBuilder tmp_builder{builder};
  tmp_builder.add_point(point_vector);
  tmp_builder.add_matrix(matrix_vector);
  FinishHomoMatrixFbsBuffer(builder, tmp_builder.Finish());
  return std::make_shared<flatbuffers::DetachedBuffer>(builder.Release());
}

void HomoMatrix::setRotationMatrix(const RotationMatrix& m) {
  std::lock_guard<std::mutex> lock{mutex_};
  matrix_ = m;
}

void HomoMatrix::setPoint(const Point& p) {
  std::lock_guard<std::mutex> lock{mutex_};
  point_ = p;
}

std::ostream& operator<<(std::ostream& out, const HomoMatrix& p) {
  out << p.point_ << p.matrix_;
  return out;
}
}  // namespace simple_msgs
