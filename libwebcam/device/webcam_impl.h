#pragma once

#include "../error/webcam_exception.h"
#include "device.h"

namespace webcam
{
	class webcam_impl
	{
	public:
		static const unsigned char MIN_CAMERA_NUMBER	= 1;
		static const unsigned char MAX_CAMERA_NUMBER	= 255;
		webcam_impl(unsigned char camera_number_, device & webcam_);
		virtual ~webcam_impl(void);

		virtual void open() = 0;
		virtual void close() = 0;
		virtual image * read() = 0;
		unsigned char get_webcam_number()const;
		virtual void release_safe() = 0;
	protected:
		unsigned char _camera_number;
		device & _device;
	};

}
