#pragma once

#include "state.h"

namespace webcam
{

	class closed : public state
	{
	public:
		closed(void);
		virtual ~closed(void);

		virtual void open(device & device_);
		virtual void close(device & device_){}
		virtual void release_safe(device & device_){}
		static closed * instance();
	};

}