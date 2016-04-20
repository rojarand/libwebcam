#include "image.h"
#include <cstring>

namespace webcam
{

	image::image()
	{
	}

	image::image(const image & source_image_){
		_video_settings = source_image_._video_settings;
		_buffer = source_image_._buffer;
	}

	image::~image()
	{
	}

	const image & image::operator=(const image & source_image_)
	{
		if (this != &source_image_)
		{
			_video_settings = source_image_._video_settings;
			_buffer = source_image_._buffer;
		}
		return *this;
	}

	void image::copy_deep(
		const unsigned char * source_data_,
		unsigned int source_data_lenght_,
		const video_settings & video_settings_)
	{
		_buffer.alloc(source_data_, source_data_lenght_);
		_video_settings = video_settings_;
	}

	void image::copy_shallow(unsigned char * source_data_,
		unsigned int source_data_lenght_,
		const video_settings & video_settings_)
	{
		buffer_data data(source_data_, source_data_lenght_);
		_buffer.attach(data);
		_video_settings = video_settings_;
	}

	const video_settings & image::get_video_settings()const
	{
		return _video_settings;
	}

	const unsigned char * image::get_data()const
	{
		return _buffer.get_data();
	}

	unsigned int image::get_data_lenght()const
	{
		return _buffer.get_length();
	}

	bool image::is_empty()const{

		return _buffer.is_empty();
	}

	void image::empty()
	{
		_buffer.free();
	}

	buffer_data image::detach()
	{
		buffer_data buffer_data = _buffer.detach();
		return buffer_data;
	}

	image * image::create_empty()
	{
		return new image();
	}
}