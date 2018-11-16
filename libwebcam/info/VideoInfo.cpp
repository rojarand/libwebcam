#include "VideoInfo.h"

namespace webcam
{
	VideoInfo::VideoInfo(const Resolution & resolution_, int format_, int image_byte_count_)
		: _resolution(resolution_)
		, _format(format_)
		, _image_byte_count(image_byte_count_)
	{

	}

	VideoInfo::VideoInfo(const VideoInfo & video_info_){

		_resolution			= video_info_.get_resolution();
		_format				= video_info_.get_format();
		_image_byte_count	= video_info_.get_image_byte_count();
	}

	const Resolution & VideoInfo::get_resolution()const
	{
		return _resolution;
	}

	int VideoInfo::get_format() const
	{
		return _format;
	}


	int VideoInfo::get_image_byte_count()const
	{
		return _image_byte_count;
	}
}
