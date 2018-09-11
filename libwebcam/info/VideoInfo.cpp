#include "VideoInfo.h"

namespace webcam
{
	VideoInfo::VideoInfo(const Resolution & resolution_, const Format & format_, int image_byte_count_)
		: _resolution(resolution_)
		, _format(format_.clone())
		, _image_byte_count(image_byte_count_)
	{

	}

	VideoInfo::VideoInfo(const VideoInfo & video_info_){

		_resolution			= video_info_.get_resolution();
		_format				= video_info_.get_format().clone();
		_image_byte_count	= video_info_.get_image_byte_count();
	}

	VideoInfo::~VideoInfo(){

		free_format();
	}

	const Resolution & VideoInfo::get_resolution()const
	{
		return _resolution;
	}

	const Format & VideoInfo::get_format()const
	{
		return *_format;
	}


	int VideoInfo::get_image_byte_count()const
	{
		return _image_byte_count;
	}

	void VideoInfo::free_format()
	{
		if(_format != nullptr){
			delete _format;
			_format = nullptr;
		}
	}
}
