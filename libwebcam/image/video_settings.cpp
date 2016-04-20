#include "video_settings.h"

namespace webcam
{

	const short resolution::DEFAULT_WIDTH = 320;
	const short resolution::DEFAULT_HEIGHT = 240;

	resolution::resolution(short width_, short height_)
		: _width(width_)
		, _height(height_)
	{

	}

	bool resolution::operator < (const resolution & resolution_)const
	{
		int area1 = _width * _height;
		int area2 = resolution_.get_width() * resolution_.get_height();
		return area1 < area2;
	}

	bool resolution::operator != (const resolution & resolution_)const
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

	short resolution::get_width()const
	{
		return _width;
	}

	short resolution::get_height()const
	{
		return _height;
	}

	void resolution::set_width(short width_)
	{
		_width = width_;
	}

	void resolution::set_height(short height_)
	{
		_height = height_;
	}

	////////////////////////////////////////////////////////

	video_settings::video_settings()
		: _format(new format_UNKN())
		, _fps(FPS_Slow)
		, _quality(Quality_Medium)
	{
	}

	video_settings::video_settings(const video_settings & video_settings_)
	{
		assign_from(video_settings_);
	}

	video_settings::~video_settings()
	{
		free_format();
	}

	const video_settings & video_settings::operator = (const video_settings & video_settings_)
	{
		if (this != &video_settings_)
		{
			assign_from(video_settings_);
		}
		return *this;
	}

	const format & video_settings::get_format()const
	{
		return *_format;
	}
	unsigned char video_settings::get_fps()const
	{
		return _fps;
	}
	unsigned int video_settings::get_height()const
	{
		return _resolution.get_height();
	}
	unsigned char video_settings::get_quality()const
	{
		return _quality;
	}
	const resolution & video_settings::get_resolution()const
	{
		return _resolution;
	}
	unsigned int video_settings::get_width()const
	{
		return _resolution.get_width();
	}

	void video_settings::set_format(const format & format_)
	{
		free_format();
		_format = format_.clone();
	}

	void video_settings::set_fps(unsigned char fps_)
	{
		_fps = fps_;
	}

	void video_settings::set_quality(unsigned char quality_)
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

	void video_settings::set_resolution(const resolution & resolution_)
	{
		_resolution = resolution_;
	}

	void video_settings::assign_from(const video_settings & video_settings_)
	{
		_format = video_settings_.get_format().clone();
		_fps = video_settings_.get_fps();
		_quality = video_settings_.get_quality();
		_resolution = video_settings_.get_resolution();
	}

	void video_settings::free_format()
	{
		if(_format != nullptr){
			delete _format;
			_format = nullptr;
		}
	}

}
