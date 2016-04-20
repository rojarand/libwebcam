#pragma once

#include "element_info_enumeration.h"
#include "video_info.h"

namespace webcam
{

	class video_info_enumeration : public element_info_enumeration<video_info>
	{
	public:
		video_info_enumeration();
		~video_info_enumeration();
	};

}