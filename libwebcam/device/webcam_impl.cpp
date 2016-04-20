#include "webcam_impl.h"

namespace webcam
{

	webcam_impl::webcam_impl(unsigned char camera_number_, device & device_)
		:_camera_number(camera_number_)
		, _device(device_)
	{
	}


	webcam_impl::~webcam_impl(void)
	{
	}

	unsigned char webcam_impl::get_webcam_number()const
	{
		return _camera_number;
	}
}
