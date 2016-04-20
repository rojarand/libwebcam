#pragma once

#include <string>
#include "../device.h"
#include "../webcam_impl.h"


namespace webcam
{
	class image;
	class state
	{
	public:
		state();
		virtual ~state(void);

		virtual void open(device & device_) = 0;
		virtual void close(device & device_) = 0;
		virtual image * read(device & device_);
		virtual void set_video_settings(device & device_, const video_settings & video_settings_);
		virtual void release_safe(device & device_) = 0;
		virtual bool is_open()const;
	protected:
		webcam_impl & get_impl(device & device_);
		void set_state(device & device_, state * state_);
	};

}
