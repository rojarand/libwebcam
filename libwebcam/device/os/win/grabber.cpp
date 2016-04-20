#include "grabber.h"

#ifdef WIN32

#include <iostream>
#include "../../../image/image.h"

namespace webcam
{

	std::chrono::milliseconds grabber::DEFAULT_FRAME_TIMEOUT(200);

	grabber::grabber()
		:_frame_timeout(DEFAULT_FRAME_TIMEOUT)
	{
	}


	grabber::~grabber()
	{
	}

	STDMETHODIMP grabber::QueryInterface(REFIID riid, void ** ppv)
	{
		if (IID_ISampleGrabberCB == riid || IID_IUnknown == riid)
		{
			*ppv = (void *) static_cast<ISampleGrabberCB*> (this);
			return NOERROR;
		}

		return E_NOINTERFACE;
	}

	STDMETHODIMP grabber::BufferCB(double sample_time_, BYTE * buf_CB_, long len_CB_)
	{
		_sync_buffer.alloc((const unsigned char*)buf_CB_, (unsigned int)len_CB_);
		go();
		return S_OK;
	}

	void grabber::set_video_settings(const video_settings & video_settings_)
	{
		_video_settings = video_settings_;
		std::chrono::milliseconds frame_timeout((1000 / _video_settings.get_fps()) * 2);
		_frame_timeout = frame_timeout;
	}

	image * grabber::grab()
	{
		wait();
		buffer_data buffer_data = _sync_buffer.detach();

		webcam::image * image = new webcam::image();
		unsigned int lenght = buffer_data.get_lenght();
		unsigned char * data = buffer_data.get_data();
		if (0 < lenght){
			image->copy_shallow(data, lenght, _video_settings);
		}
		return image;
	}

	void grabber::wait()
	{
		std::unique_lock<std::mutex> lock(cv_mutex);
		cv.wait_for(lock, _frame_timeout);
	}

	void grabber::go()
	{
		cv.notify_one();
	}
}

#endif
