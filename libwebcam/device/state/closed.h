#pragma once

#include "state.h"

namespace webcam
{

	class closed : public state
	{
	public:
		closed(void);
		virtual ~closed(void);

		virtual void open(Device & device_);
		virtual void close(Device & device_){}
		virtual void release_safe(Device & device_){}
		static closed * instance();
	};

}