#pragma once

#include "image/VideoSettings.h"

namespace webcam
{
	class VideoInfo
	{
	public:
		VideoInfo(const Resolutions & resolutions_, int format_, int image_byte_count_);
		VideoInfo(const VideoInfo & video_info_);

		const Resolutions & get_resolutions()const;
		int get_format()const;
		int get_image_byte_count()const;
	private:
		Resolutions _resolutions;
		int _format;
		int _image_byte_count;
	};
}
