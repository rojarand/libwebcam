#pragma once

#include "image/VideoSettings.h"

namespace webcam
{
	class VideoInfo
	{
	public:
		VideoInfo(const Resolution & resolution_, const Format & format_, int image_byte_count_);
		VideoInfo(const VideoInfo & video_info_);
		~VideoInfo();
		const Resolution & get_resolution()const;
		const Format & get_format()const;
		int get_image_byte_count()const;
	private:
		void free_format();
		Resolution _resolution;
		Format * _format;
		int _image_byte_count;
	};
}
