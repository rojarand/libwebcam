#include "video_info.h"

namespace webcam
{
	video_info::video_info(const resolution & resolution_, const format & format_, int image_byte_count_)
		: _resolution(resolution_)
		, _format(format_.clone())
		, _image_byte_count(image_byte_count_)
	{

	}

	video_info::video_info(const video_info & video_info_){

		_resolution			= video_info_.get_resolution();
		_format				= video_info_.get_format().clone();
		_image_byte_count	= video_info_.get_image_byte_count();
	}

	video_info::~video_info(){

		free_format();
	}

	const resolution & video_info::get_resolution()const
	{
		return _resolution;
	}

	const format & video_info::get_format()const
	{
		return *_format;
	}


	int video_info::get_image_byte_count()const
	{
		return _image_byte_count;
	}

	void video_info::free_format()
	{
		if(_format != nullptr){
			delete _format;
			_format = nullptr;
		}
	}
}
