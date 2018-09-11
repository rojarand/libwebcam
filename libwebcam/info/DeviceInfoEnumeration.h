#pragma once

#include "ElementInfoEnumeration.h"
#include "DeviceInfo.h"

namespace webcam
{

	class DeviceInfoEnumeration : public ElementInfoEnumeration<DeviceInfo>
	{
	public:
		DeviceInfoEnumeration();
		~DeviceInfoEnumeration();
	};

}