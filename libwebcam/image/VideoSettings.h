#pragma once

#include "Format.h"
#ifndef WIN32
#include <linux/videodev2.h>
#else
#include <wmsdkidl.h>
#endif

namespace webcam
{

	enum Quality
	{
		Quality_Poor = 1,
		Quality_Medium = 15,
		Quality_Good = 50,
		Quality_Best = 95,
	};

	enum FPS
	{
		FPS_VerySlow = 1,
		FPS_Slow = 6,
		FPS_Medium = 12,
		FPS_Fluent = 16,
		FPS_Default = FPS_Slow,
	};

	class Resolution
	{
	public:
		static const short DEFAULT_WIDTH;
		static const short DEFAULT_HEIGHT;
		Resolution(short width_ = DEFAULT_WIDTH, short height_ = DEFAULT_HEIGHT);
		bool operator < (const Resolution & resolution_)const;
		bool operator != (const Resolution & resolution_)const;
		short get_width()const;
		short get_height()const;

		void set_width(short width_);
		void set_height(short height_);
	protected:
		short _width;
		short _height;
	};

	class VideoSettings
	{
	public:
		VideoSettings();
		VideoSettings(const VideoSettings & video_settings_);
		~VideoSettings();

		const VideoSettings & operator=(const VideoSettings & video_settings_);

		const Format & get_format()const;
		unsigned int get_fps()const;
		unsigned int get_height()const;
		unsigned char get_quality()const;
		const Resolution & get_resolution()const;
		unsigned int get_width()const;

		void set_format(const Format & format_);
		void set_fps(unsigned int fps_);
		void set_quality(unsigned char quality_);
		void set_resolution(const Resolution & resolution_);
	protected:
		void free_format();
		void assign_from(const VideoSettings & video_settings_);
		Format * _format;
		unsigned int _fps;
		unsigned char _quality;
		Resolution _resolution;
	};

}
