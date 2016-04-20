#pragma once

#ifdef WIN32

#include <condition_variable>
#include <chrono>

#include "qedit.h"
#include "../../../utils/sync_buffer.h"
#include "../../../image/video_settings.h"

namespace webcam
{

	class image;
	class grabber : public ISampleGrabberCB
	{
	public:
		grabber();
		~grabber();

		//
		// Fake out any COM ref counting
		//
		STDMETHODIMP_(ULONG) AddRef() { return 2; }
		STDMETHODIMP_(ULONG) Release() { return 1; }

		STDMETHODIMP QueryInterface(REFIID riid, void ** ppv);
		// We don't implement this one
		//
		STDMETHODIMP SampleCB(double SampleTime, IMediaSample * pSample)
		{
			return 0;
		}

		// The sample grabber is calling us back on its deliver thread.
		// This is NOT the main app thread!
		//
		STDMETHODIMP BufferCB(double sample_time_, BYTE * buf_CB_, long len_CB_);

		void set_video_settings(const video_settings & video_settings_);
		virtual image * grab();
	private:
		void wait();
		void go();
		std::mutex cv_mutex;
		std::condition_variable cv;
		static std::chrono::milliseconds DEFAULT_FRAME_TIMEOUT;
		std::chrono::milliseconds _frame_timeout;

		webcam::video_settings _video_settings;
		sync_buffer _sync_buffer;
	};

}

#endif
