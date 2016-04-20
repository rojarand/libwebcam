#pragma once

#include "element_info_enumeration.h"
#include "device_info.h"

namespace webcam
{

	class device_info_enumeration : public element_info_enumeration<device_info>
	{
	public:
		device_info_enumeration();
		~device_info_enumeration();
	};

}