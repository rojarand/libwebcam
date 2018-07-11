#pragma once

#include "../image/video_settings.h"

namespace webcam
{
	class video_info
	{
	public:
		video_info(const resolution & resolution_, const Format & format_, int image_byte_count_);
		video_info(const video_info & video_info_);
		~video_info();
		const resolution & get_resolution()const;
		const Format & get_format()const;
		int get_image_byte_count()const;
	private:
		void free_format();
		resolution _resolution;
		Format * _format;
		int _image_byte_count;
	};
}
