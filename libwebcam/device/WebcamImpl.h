#pragma once

#include "../error/webcam_exception.h"
#include "Device.h"

namespace webcam
{
	class WebcamImpl
	{
	public:
		static const unsigned char MIN_CAMERA_NUMBER	= 1;
		static const unsigned char MAX_CAMERA_NUMBER	= 255;
		WebcamImpl(unsigned char camera_number_, Device & webcam_);
		virtual ~WebcamImpl(void);

		virtual void open() = 0;
		virtual void close() = 0;
		virtual Image * read() = 0;
		unsigned char get_webcam_number()const;
		virtual void release_safe() = 0;
	protected:
		unsigned char _camera_number;
		Device & _device;
	};

}
