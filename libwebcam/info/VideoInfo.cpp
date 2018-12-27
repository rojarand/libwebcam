#include "VideoInfo.h"

namespace webcam
{
	VideoInfo::VideoInfo(const Resolutions & resolutions_, int format_, int image_byte_count_)
		: _resolutions(resolutions_)
		, _format(format_)
		, _image_byte_count(image_byte_count_)
	{

	}

	VideoInfo::VideoInfo(const VideoInfo & video_info_){

		_resolutions		= video_info_.get_resolutions();
		_format				= video_info_.get_format();
		_image_byte_count	= video_info_.get_image_byte_count();
	}

	const Resolutions & VideoInfo::get_resolutions()const
	{
		return _resolutions;
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
