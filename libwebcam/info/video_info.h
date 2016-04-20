#pragma once

#include "../image/video_settings.h"

namespace webcam
{
	class video_info
	{
	public:
		video_info(const resolution & resolution_, const format & format_, int image_byte_count_);
		video_info(const video_info & video_info_);
		~video_info();
		const resolution & get_resolution()const;
		const format & get_format()const;
		int get_image_byte_count()const;
	private:
		void free_format();
		resolution _resolution;
		format * _format;
		int _image_byte_count;
	};
}
