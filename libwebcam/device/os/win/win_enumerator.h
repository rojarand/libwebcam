#ifdef WIN32
#pragma once

#include "device_info_provider.h"
#include "../../../info/device_info_enumeration.h"
#include "../../../error/enumerator_exception.h"

namespace webcam
{

	class enumerator
	{
	public:
		~enumerator();
		static enumerator create();
		device_info_provider next();
		static device_info_enumeration enumerate();
	private:
		enumerator(IEnumMoniker * enum_moniker_, bool cleanUp_);
		IEnumMoniker * _enum_moniker;
		bool cleanUp;
	};
}
#endif
