#include "WebcamImpl.h"

namespace webcam
{

	WebcamImpl::WebcamImpl(unsigned char camera_number_, Device & device_)
		:_camera_number(camera_number_)
		, _device(device_)
	{
	}


	WebcamImpl::~WebcamImpl(void)
	{
	}

	unsigned char WebcamImpl::get_webcam_number()const
	{
		return _camera_number;
	}
}
