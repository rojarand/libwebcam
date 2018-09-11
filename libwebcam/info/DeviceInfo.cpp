#include "DeviceInfo.h"

namespace webcam
{
	DeviceInfo::DeviceInfo()
	{
	}

	DeviceInfo::~DeviceInfo()
	{
	}

	const VideoInfoEnumeration & DeviceInfo::get_video_info_enumeration()const
	{
		return _video_info_enumeration;
	}

	void DeviceInfo::set_video_info_enumeration(const VideoInfoEnumeration & video_info_enumeration_)
	{
		_video_info_enumeration = video_info_enumeration_;
	}

	const ModelInfo & DeviceInfo::get_model_info()const
	{
		return _model_info;
	}

	void DeviceInfo::set_model_info(ModelInfo & model_info_)
	{
		_model_info = model_info_;
	}

	bool DeviceInfo::supports_format(const Format & format_)const
	{
		size_t count = _video_info_enumeration.count();
		for (size_t index = 0; index < count; index++){
			const webcam::VideoInfo & video_info = _video_info_enumeration.get(index);
			if (video_info.get_format() == format_)
			{
				return true;
			}
		}
		return false;
	}

	Resolution DeviceInfo::get_highest_resolution(const Format & format_)const
	{
		Resolution current_resolution(0, 0);
		size_t count = _video_info_enumeration.count();
		for (size_t index = 0; index < count; index++){
			const webcam::VideoInfo & video_info = _video_info_enumeration.get(index);
			if (video_info.get_format() == format_)
			{
				const Resolution & new_resolution = video_info.get_resolution();
				if (current_resolution < new_resolution)
				{
					current_resolution = new_resolution;
				}
			}
		}
		return current_resolution;
	}
}