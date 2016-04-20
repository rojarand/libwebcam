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
		virtual void open(device & device_){}
		virtual void close(device & device_);
		virtual image * read(device & device_);
		virtual void set_video_settings(device & device_, const video_settings & video_settings_);
		virtual void release_safe(device & device_);
		virtual bool is_open()const;
	};

}
