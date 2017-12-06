
#include "simple_msgs/rotation_matrix.h"

namespace simple_msgs
{
RotationMatrix::RotationMatrix(const uint8_t* data)
{
  auto r = GetRotationMatrixFbs(data);
  matrix_ =
      std::array<double, 9>{r->r11(), r->r12(), r->r13(), r->r21(), r->r22(), r->r23(), r->r31(), r->r32(), r->r33()};
  modified_ = true;
}

RotationMatrix& RotationMatrix::operator=(const RotationMatrix& m)
{
  if (this != std::addressof(m))
  {
    matrix_ = m.matrix_;
    modified_ = true;
  }
  return *this;
}

bool RotationMatrix::operator==(const RotationMatrix& m) const
{
  return matrix_ == m.matrix_;
}

bool RotationMatrix::operator!=(const RotationMatrix& m) const
{
  return !(*this == m);
}

uint8_t* RotationMatrix::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (modified_)
  {
    builder_->Clear();
    RotationMatrixFbsBuilder tmp_builder(*builder_);
    tmp_builder.add_r11(matrix_[0]);
    tmp_builder.add_r12(matrix_[1]);
    tmp_builder.add_r13(matrix_[2]);
    tmp_builder.add_r21(matrix_[3]);
    tmp_builder.add_r22(matrix_[4]);
    tmp_builder.add_r23(matrix_[5]);
    tmp_builder.add_r31(matrix_[6]);
    tmp_builder.add_r32(matrix_[7]);
    tmp_builder.add_r33(matrix_[8]);
    FinishRotationMatrixFbsBuffer(*builder_, tmp_builder.Finish());
    modified_ = false;
  }
  return builder_->GetBufferPointer();
}

RotationMatrix RotationMatrix::getTranspose() const
{
  return RotationMatrix(matrix_[0], matrix_[3], matrix_[6], matrix_[1], matrix_[4], matrix_[7], matrix_[2], matrix_[5],
                        matrix_[8]);
}

std::array<double, 3> RotationMatrix::getRow(int row_index) const
{
  if (0 <= row_index && row_index <= 2)
  {
    return std::array<double, 3>{matrix_[row_index * 3], matrix_[row_index * 3 + 1], matrix_[row_index * 3 + 2]};
  }
  else
  {
    throw std::out_of_range("Index out of range [0,2]");
  }
}

std::array<double, 3> RotationMatrix::getColumn(int column_index) const
{
  if (0 <= column_index && column_index <= 2)
  {
    return std::array<double, 3>{matrix_[column_index], matrix_[3 * 1 + column_index], matrix_[3 * 2 + column_index]};
  }
  else
  {
    throw std::out_of_range("Index out of range [0,2]");
  }
}
void RotationMatrix::setRow(int row_index, const std::array<double, 3>& values)
{
  for (auto i = 0; i < values.size(); ++i)
  {
    matrix_[row_index * 3 + i] = values[i];
  }
}

void RotationMatrix::setColumn(int column_index, const std::array<double, 3>& values)
{
  for (auto i = 0; i < values.size(); ++i)
  {
    matrix_[3 * i + column_index] = values[i];
  }
}

std::ostream& operator<<(std::ostream& out, const RotationMatrix& q)
{
  out << "RotationMatrix \n \t" << q.matrix_[0] << " " << q.matrix_[1] << " " << q.matrix_[2] << "\n"
      << q.matrix_[3] << " " << q.matrix_[4] << " " << q.matrix_[5] << "\n"
      << q.matrix_[6] << " " << q.matrix_[7] << " " << q.matrix_[8] << "\n";

  return out;
}
}
