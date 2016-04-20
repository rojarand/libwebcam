#include "device_info.h"

namespace webcam
{
	device_info::device_info()
	{
	}

	device_info::~device_info()
	{
	}

	const video_info_enumeration & device_info::get_video_info_enumeration()const
	{
		return _video_info_enumeration;
	}

	void device_info::set_video_info_enumeration(const video_info_enumeration & video_info_enumeration_)
	{
		_video_info_enumeration = video_info_enumeration_;
	}

	const model_info & device_info::get_model_info()const
	{
		return _model_info;
	}

	void device_info::set_model_info(model_info & model_info_)
	{
		_model_info = model_info_;
	}

	bool device_info::supports_format(const format & format_)const
	{
		size_t count = _video_info_enumeration.count();
		for (size_t index = 0; index < count; index++){
			const webcam::video_info & video_info = _video_info_enumeration.get(index);
			if (video_info.get_format() == format_)
			{
				return true;
			}
		}
		return false;
	}

	resolution device_info::get_highest_resolution(const format & format_)const
	{
		resolution current_resolution(0, 0);
		size_t count = _video_info_enumeration.count();
		for (size_t index = 0; index < count; index++){
			const webcam::video_info & video_info = _video_info_enumeration.get(index);
			if (video_info.get_format() == format_)
			{
				const resolution & new_resolution = video_info.get_resolution();
				if (current_resolution < new_resolution)
				{
					current_resolution = new_resolution;
				}
			}
		}
		return current_resolution;
	}
}