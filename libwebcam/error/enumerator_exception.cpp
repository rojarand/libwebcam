#include "enumerator_exception.h"

namespace webcam
{
	enumerator_exception::enumerator_exception(const char * what_, int error_number_)
		:webcam_exception(what_, error_number_)
	{
	}


	enumerator_exception::~enumerator_exception()throw()
	{
	}

	system_device_enumerator_exception::system_device_enumerator_exception(const char * what_, int error_number_)
		:enumerator_exception(what_, error_number_)
	{

	}

	video_device_enumerator_exception::video_device_enumerator_exception(const char * what_, int error_number_)
		: enumerator_exception(what_, error_number_)
	{

	}
}
