#pragma once

#include <string>
#include "ModelInfo.h"
#include "VideoInfoEnumeration.h"

namespace webcam
{
    class DeviceInfo
	{
	public:
		DeviceInfo();
		~DeviceInfo();

		const VideoInfoEnumeration & get_video_info_enumeration()const;
		void set_video_info_enumeration(const VideoInfoEnumeration & video_info_enumeration_);

		const ModelInfo & get_model_info()const;
		void set_model_info(ModelInfo & model_info_);
		bool supports_format(const Format & format_)const;
		Resolution get_highest_resolution(const Format & format_)const;

	private:
		ModelInfo _model_info;
		VideoInfoEnumeration _video_info_enumeration;
	};
}