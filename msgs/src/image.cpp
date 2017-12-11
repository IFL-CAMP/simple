
#include "simple_msgs/image.h"

namespace simple_msgs
{
	
template <>
data Image<uint8_t>::getDataUnionType() const
{
  return data_uint8_type;
}

template <>
data Image<int16_t>::getDataUnionType() const
{
  return data_int16_type;
}

template <>
data Image<float>::getDataUnionType() const
{
  return data_float_type;
}

template <>
data Image<double>::getDataUnionType() const
{
  return data_double_type;
}

template <>
flatbuffers::Offset<void> Image<uint8_t>::getDataUnionElem() const
{
  return Createuint8_type(*builder_, builder_->CreateVector(data_, data_size_)).Union();
}

template <>
flatbuffers::Offset<void> Image<int16_t>::getDataUnionElem() const
{
  return Createint16_type(*builder_, builder_->CreateVector(data_, data_size_)).Union();
}

template <>
flatbuffers::Offset<void> Image<float>::getDataUnionElem() const
{
  return Createfloat_type(*builder_, builder_->CreateVector(data_, data_size_)).Union();
}

template <>
flatbuffers::Offset<void> Image<double>::getDataUnionElem() const
{
  return Createdouble_type(*builder_, builder_->CreateVector(data_, data_size_)).Union();
}


template <>
Image<uint8_t>& Image<uint8_t>::operator=(const uint8_t* data)
{
	std::lock_guard<std::mutex> lock(mutex_);
	auto image_data = GetImageFbs(data);

	Image<uint8_t>::fillPartialImage(image_data);

	// Set the Image data according to the right date type.
	data_ = (static_cast<const uint8_type*>(image_data->image()))->raw()->data();

	return *this;
}

template <>
Image<int16_t>& Image<int16_t>::operator=(const uint8_t* data)
{
	std::lock_guard<std::mutex> lock(mutex_);
	auto image_data = GetImageFbs(data);

	Image<int16_t>::fillPartialImage(image_data);

	// Set the Image data according to the right date type.
	data_ = (static_cast<const int16_type*>(image_data->image()))->raw()->data();

	return *this;
}

template <>
Image<double>& Image<double>::operator=(const uint8_t* data)
{
	std::lock_guard<std::mutex> lock(mutex_);
	auto image_data = GetImageFbs(data);

	Image<double>::fillPartialImage(image_data);

	// Set the Image data according to the right date type.
	data_ = (static_cast<const double_type*>(image_data->image()))->raw()->data();

	return *this;
}

template <>
Image<float>& Image<float>::operator=(const uint8_t* data)
{
	std::lock_guard<std::mutex> lock(mutex_);
	auto image_data = GetImageFbs(data);

	Image<float>::fillPartialImage(image_data);

	// Set the Image data according to the right date type.
	data_ = (static_cast<const float_type*>(image_data->image()))->raw()->data();

	return *this;
}

template <>
Image<uint8_t>::Image(const uint8_t* data)
	: GenericMessage()
{
	auto image_data = GetImageFbs(data);

	Image<uint8_t>::fillPartialImage(image_data);
	// Set the Image data according to the right date type.
	data_ = (static_cast<const uint8_type*>(image_data->image()))->raw()->data();
}

template <>
Image<int16_t>::Image(const uint8_t* data)
	: GenericMessage()
{
	auto image_data = GetImageFbs(data);

	Image<int16_t>::fillPartialImage(image_data);
	// Set the Image data according to the right date type.
	data_ = (static_cast<const int16_type*>(image_data->image()))->raw()->data();
}

template <>
Image<double>::Image(const uint8_t* data)
	: GenericMessage()
{
	auto image_data = GetImageFbs(data);

	Image<double>::fillPartialImage(image_data);
	// Set the Image data according to the right date type.
	data_ = (static_cast<const double_type*>(image_data->image()))->raw()->data();
}

template <>
Image<float>::Image(const uint8_t* data)
	: GenericMessage()
{
	auto image_data = GetImageFbs(data);

	Image<float>::fillPartialImage(image_data);
	// Set the Image data according to the right date type.
	data_ = (static_cast<const float_type*>(image_data->image()))->raw()->data();
}


}
