#pragma once

#include <vector>

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

	class Shape {
	public:
		int width,height;

		Shape( int  width, int height);
		Shape();
	};

	class Resolutions {
	public:
		Resolutions();

		// Selects the allowed resolution which is the closest match to the requested
		bool find_best_match( int request_width , int request_height ,
							  int &selected_width , int &selected_height ) const;

		std::vector<Shape> _resolutions;
		int _width_min,_width_max,_width_step;
		int _height_min,_height_max,_height_step;
	};

	template <class T>
	class Parameter {
	public:
		Parameter() {
			manual = false;
			value = 0;
		}

		bool is_manual() {
			return manual;
		}
		T get_value() {
			return value;
		}

		void set_manual( bool v ) {
			this->manual = v;
		}
		void set_value( T value ) {
			this->value = value;
		}

	protected:
		bool manual;
		T value;
	};

	class VideoSettings
	{
	public:
		VideoSettings();
		VideoSettings(const VideoSettings & video_settings_);

		const VideoSettings & operator=(const VideoSettings & video_settings_);

		int get_format() const;
		unsigned int get_fps()const;
		unsigned char get_quality()const;
		int get_width()const;
		int get_height()const;
		const Parameter<int>& get_focus() const;
        const Parameter<int>& get_exposure() const;

        void set_format(int format_);
		void set_fps(unsigned int fps_);
		void set_quality(unsigned char quality_);
		void set_resolution(const Shape & resolution_);
	protected:
		void assign_from(const VideoSettings & video_settings_);
		int _format;
		unsigned int _fps;
		unsigned char _quality;
		int _width;
		int _height;
        Parameter<int> focus;
        Parameter<int> exposure;
    };

}
