#include "VideoSettings.h"

namespace webcam
{

	const short Resolution::DEFAULT_WIDTH = 320;
	const short Resolution::DEFAULT_HEIGHT = 240;

	Resolution::Resolution(short width_, short height_)
		: _width(width_)
		, _height(height_)
	{

	}

	bool Resolution::operator < (const Resolution & resolution_)const
	{
		int area1 = _width * _height;
		int area2 = resolution_.get_width() * resolution_.get_height();
		return area1 < area2;
	}

	bool Resolution::operator != (const Resolution & resolution_)const
	{
		if (_width != resolution_.get_width())
		{
			return true;
		}

		if (_height != resolution_.get_height())
		{
			return true;
		}

		return false;
	}

	short Resolution::get_width()const
	{
		return _width;
	}

	short Resolution::get_height()const
	{
		return _height;
	}

	void Resolution::set_width(short width_)
	{
		_width = width_;
	}

	void Resolution::set_height(short height_)
	{
		_height = height_;
	}

	////////////////////////////////////////////////////////

	VideoSettings::VideoSettings()
		: _format(0)
		, _fps(FPS_Slow)
		, _quality(Quality_Medium)
	{
	}

	VideoSettings::VideoSettings(const VideoSettings & video_settings_)
	{
		assign_from(video_settings_);
	}


	const VideoSettings & VideoSettings::operator = (const VideoSettings & video_settings_)
	{
		if (this != &video_settings_)
		{
			assign_from(video_settings_);
		}
		return *this;
	}

	int VideoSettings::get_format()const
	{
		return _format;
	}
	unsigned int VideoSettings::get_fps()const
	{
		return _fps;
	}
	unsigned int VideoSettings::get_height()const
	{
		return _resolution.get_height();
	}
	unsigned char VideoSettings::get_quality()const
	{
		return _quality;
	}
	const Resolution & VideoSettings::get_resolution()const
	{
		return _resolution;
	}
	unsigned int VideoSettings::get_width()const
	{
		return _resolution.get_width();
	}

	void VideoSettings::set_format(int format_)
	{
		_format = format_;
	}

	void VideoSettings::set_fps(unsigned int fps_)
	{
		_fps = fps_;
	}

	void VideoSettings::set_quality(unsigned char quality_)
	{
		if (quality_ < Quality_Poor)
		{
			_quality = Quality_Poor;
		}
		else if (Quality_Best < quality_)
		{
			_quality = Quality_Best;
		}
		else
		{
			_quality = quality_;
		}
	}

	void VideoSettings::set_resolution(const Resolution & resolution_)
	{
		_resolution = resolution_;
	}

	void VideoSettings::assign_from(const VideoSettings & video_settings_)
	{
		_format = video_settings_.get_format();
		_fps = video_settings_.get_fps();
		_quality = video_settings_.get_quality();
		_resolution = video_settings_.get_resolution();
	}
}
