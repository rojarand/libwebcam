#include "Image.h"
#include <cstring>

namespace webcam
{

	Image::Image()
	{
	}

	Image::Image(const Image & source_image_){
		_video_settings = source_image_._video_settings;
		_buffer = source_image_._buffer;
	}

	Image::~Image()
	{
	}

	const Image & Image::operator=(const Image & source_image_)
	{
		if (this != &source_image_)
		{
			_video_settings = source_image_._video_settings;
			_buffer = source_image_._buffer;
		}
		return *this;
	}

	void Image::copy_deep(
		const unsigned char * source_data_,
		unsigned int source_data_lenght_,
		const VideoSettings & video_settings_)
	{
		_buffer.alloc(source_data_, source_data_lenght_);
		_video_settings = video_settings_;
	}

	void Image::copy_shallow(unsigned char * source_data_,
		unsigned int source_data_lenght_,
		const VideoSettings & video_settings_)
	{
		BufferData data(source_data_, source_data_lenght_);
		_buffer.attach(data);
		_video_settings = video_settings_;
	}

	const VideoSettings & Image::get_video_settings()const
	{
		return _video_settings;
	}

	const unsigned char * Image::get_data()const
	{
		return _buffer.get_data();
	}
	unsigned char * Image::get_data()
	{
		return _buffer.get_data();
	}

	unsigned int Image::get_data_length()const
	{
		return _buffer.get_length();
	}

	bool Image::is_empty()const{

		return _buffer.is_empty();
	}

	void Image::empty()
	{
		_buffer.free();
	}

	BufferData Image::detach()
	{
		BufferData buffer_data = _buffer.detach();
		return buffer_data;
	}

	Image * Image::create_empty()
	{
		return new Image();
	}
}