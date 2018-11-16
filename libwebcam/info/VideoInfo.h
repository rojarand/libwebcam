#pragma once

#include "image/VideoSettings.h"

namespace webcam
{
	class VideoInfo
	{
	public:
		VideoInfo(const Resolution & resolution_, int format_, int image_byte_count_);
		VideoInfo(const VideoInfo & video_info_);

		const Resolution & get_resolution()const;
		int get_format()const;
		int get_image_byte_count()const;
	private:
		Resolution _resolution;
		int _format;
		int _image_byte_count;
	};
}
