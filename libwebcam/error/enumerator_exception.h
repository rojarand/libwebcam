#pragma once

#include "webcam_exception.h"

namespace webcam
{
	class enumerator_exception : public webcam_exception
	{
	public:
		enumerator_exception(const char * what_, int error_number_);
		virtual ~enumerator_exception()throw();
	};

	class system_device_enumerator_exception : public enumerator_exception
	{
	public:
		system_device_enumerator_exception(const char * what_, int error_number_);
	};

	class video_device_enumerator_exception : public enumerator_exception
	{
	public:
		video_device_enumerator_exception(const char * what_, int error_number_);
	};

}
