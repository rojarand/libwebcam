#pragma once

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
		static const int DEFAULT_WIDTH;
		static const int DEFAULT_HEIGHT;
		Resolution(int width_ = DEFAULT_WIDTH, int height_ = DEFAULT_HEIGHT);
		bool operator < (const Resolution & resolution_)const;
		bool operator != (const Resolution & resolution_)const;
		int get_width()const;
		int get_height()const;

		void set_width(int width_);
		void set_height(int height_);
	protected:
		int _width;
		int _height;
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
		unsigned int get_height()const;
		unsigned char get_quality()const;
		const Resolution & get_resolution()const;
		unsigned int get_width()const;
		const Parameter<int>& get_focus() const;
        const Parameter<int>& get_exposure() const;

        void set_format(int format_);
		void set_fps(unsigned int fps_);
		void set_quality(unsigned char quality_);
		void set_resolution(const Resolution & resolution_);
	protected:
		void assign_from(const VideoSettings & video_settings_);
		int _format;
		unsigned int _fps;
		unsigned char _quality;
		Resolution _resolution;
        Parameter<int> focus;
        Parameter<int> exposure;
    };

}
