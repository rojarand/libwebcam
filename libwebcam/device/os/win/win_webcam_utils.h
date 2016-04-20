#pragma once

#ifdef WIN32

#include <dshow.h>

namespace webcam
{

	class win_webcam_utils
	{
	public:
		~win_webcam_utils();


		static HRESULT get_pin(IBaseFilter * filter_, PIN_DIRECTION dirrequired_, int num_, IPin ** pin_);
		static IPin * get_in_pin(IBaseFilter * filter_, int nPin_);
		static IPin * get_out_pin(IBaseFilter * filter_, int nPin_);
	private:
		win_webcam_utils();
	};

}

#endif
