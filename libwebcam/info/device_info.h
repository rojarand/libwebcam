#pragma once

#include <string>
#include "model_info.h"
#include "video_info_enumeration.h"

namespace webcam
{
	class device_info
	{
	public:
		device_info();
		~device_info();

		const video_info_enumeration & get_video_info_enumeration()const;
		void set_video_info_enumeration(const video_info_enumeration & video_info_enumeration_);

		const model_info & get_model_info()const;
		void set_model_info(model_info & model_info_);
		bool supports_format(const Format & format_)const;
		resolution get_highest_resolution(const Format & format_)const;
	private:
		model_info _model_info;
		video_info_enumeration _video_info_enumeration;
	};
}