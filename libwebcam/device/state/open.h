#pragma once

#include "state.h"

namespace webcam
{

	class _open : public state
	{
	public:
		_open(void);
		~_open(void);

		static _open * instance();
		virtual void open(Device & device_){}
		virtual void close(Device & device_);
		virtual Image * read(Device & device_);
		virtual void set_video_settings(Device & device_, const VideoSettings & video_settings_);
		virtual void release_safe(Device & device_);
		virtual bool is_open()const;
	};

}
