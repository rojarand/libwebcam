#pragma once

#include "ElementInfoEnumeration.h"
#include "VideoInfo.h"

namespace webcam
{

	class VideoInfoEnumeration : public ElementInfoEnumeration<VideoInfo>
	{
	public:
		VideoInfoEnumeration();
		~VideoInfoEnumeration();
	};

}