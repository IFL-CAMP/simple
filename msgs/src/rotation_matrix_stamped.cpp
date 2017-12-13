
#include "simple_msgs/rotation_matrix_stamped.h"

namespace simple_msgs
{
RotationMatrixStamped::RotationMatrixStamped()
  : GenericMessage()
{
}

RotationMatrixStamped::RotationMatrixStamped(const RotationMatrix& rotation_matrix, const Header& header)
  : GenericMessage()
  , rotation_matrix_(rotation_matrix)
  , header_(header)
{
}

RotationMatrixStamped::RotationMatrixStamped(const uint8_t* data)
  : GenericMessage()
  , rotation_matrix_(GetRotationMatrixStampedFbs(data)->rotation_matrix()->data())
  , header_(GetRotationMatrixStampedFbs(data)->header()->data())
{
}

RotationMatrixStamped::RotationMatrixStamped(const RotationMatrixStamped& m)
  : RotationMatrixStamped(m.rotation_matrix_,m.header_)
{
}

RotationMatrixStamped::RotationMatrixStamped(RotationMatrixStamped&& m)
  : RotationMatrixStamped(std::move(m.rotation_matrix_),std::move(m.header_))
{
}

RotationMatrixStamped& RotationMatrixStamped::operator=(const RotationMatrixStamped& other)
{
  if (this != std::addressof(other))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    rotation_matrix_ = other.rotation_matrix_;
	header_ = other.header_;
    modified_ = true;
  }
  return *this;
}

RotationMatrixStamped& RotationMatrixStamped::operator=(RotationMatrixStamped&& other)
{
  if (this != std::addressof(other))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    rotation_matrix_ = std::move(other.rotation_matrix_);
	header_ = std::move(other.header_);
    modified_ = true;
  }
  return *this;
}

RotationMatrixStamped& RotationMatrixStamped::operator=(const uint8_t* data)
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto matrix = GetRotationMatrixStampedFbs(data);
  rotation_matrix_ = RotationMatrix(matrix->rotation_matrix()->data());
  header_ = Header(matrix->header()->data());
  modified_ = true;

  return *this;
}

uint8_t* RotationMatrixStamped::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (modified_)
  {
    builder_->Clear();
	auto rotation_matrix_vector = builder_->CreateVector(rotation_matrix_.getBufferData(), rotation_matrix_.getBufferSize());
	auto header_vector = builder_->CreateVector(header_.getBufferData(), header_.getBufferSize());
    RotationMatrixStampedFbsBuilder tmp_builder(*builder_);
	tmp_builder.add_rotation_matrix(rotation_matrix_vector);
	tmp_builder.add_header(header_vector);
    FinishRotationMatrixStampedFbsBuffer(*builder_, tmp_builder.Finish());
    modified_ = false;
  }
  return builder_->GetBufferPointer();
}

std::ostream& operator<<(std::ostream& out, const RotationMatrixStamped& q)
{
	out << q.header_ << q.rotation_matrix_;

  return out;
}
}  // namespace simple_msgs
