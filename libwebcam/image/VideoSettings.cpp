#include <algorithm>
#include "VideoSettings.h"

namespace webcam
{
    Shape::Shape( int  width, int height) {
        this->width = width;
        this->height = height;
    }
    Shape::Shape() : width(0),height(0) {

    }

	Resolutions::Resolutions(  ) : _width_min(0), _width_max(0), _width_step(0),
								   _height_min(0), _height_max(0), _height_step(0)
	{
	}

	bool Resolutions::find_best_match( int request_width , int request_height ,
									   int &selected_width , int &selected_height ) const {
		if(!_resolutions.empty()) {
            // minimize the difference in area
            int best_score = -1;
            for( size_t i = 0; i < this->_resolutions.size(); i++ ) {
                const Shape &shape = _resolutions.at(i);

                int dw = request_width-shape.width;
                int dh = request_height-shape.height;
                int score = dw*dw + dh*dh;

                if( best_score == -1 || best_score > score ) {
                    selected_width = shape.width;
                    selected_height = shape.height;
                    best_score = score;
                }
            }

            return best_score != -1;
        } else if(_width_step > 0 && _height_step > 0  ){
            selected_width = _width_step*((request_width-_width_min)/_width_step) + _width_min;
            selected_height = _height_step*((request_height-_height_min)/_height_step) + _height_min;

            selected_width = std::min(selected_width,_width_max);
            selected_height = std::min(selected_height,_height_max);
            return true;
        }
		return false;
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
	int VideoSettings::get_height()const
	{
		return _height;
	}
	unsigned char VideoSettings::get_quality()const
	{
		return _quality;
	}

	int VideoSettings::get_width()const
	{
		return _width;
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

	void VideoSettings::set_resolution(const Shape & resolution_)
	{
		this->_width = resolution_.width;
		this->_height = resolution_.height;
	}

	void VideoSettings::assign_from(const VideoSettings & video_settings_)
	{
		_format = video_settings_.get_format();
		_fps = video_settings_.get_fps();
		_quality = video_settings_.get_quality();
		_width = video_settings_.get_width();
		_height = video_settings_.get_height();
	}
}
