#pragma once

#include <string>
#include "device/Device.h"
#include "device/WebcamImpl.h"


namespace webcam
{
	class Image;
	class state
	{
	public:
		state();
		virtual ~state(void);

		virtual void open(Device & device_) = 0;
		virtual void close(Device & device_) = 0;
		virtual Image * read(Device & device_);
		virtual void set_video_settings(Device & device_, const VideoSettings & video_settings_);
		virtual void release_safe(Device & device_) = 0;
		virtual bool is_open()const;
	protected:
		WebcamImpl & get_impl(Device & device_);
		void set_state(Device & device_, state * state_);
	};

}
