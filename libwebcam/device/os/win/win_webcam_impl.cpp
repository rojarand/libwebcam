/*
 * win_web_cam.cpp
 *
 *  Created on: Feb 11, 2013
 *      Author: zebul
 */

#ifdef WIN32


//#import "qedit.dll" raw_interfaces_only named_guids
//#import "libid:78530B68-61F9-11D2-8CAD-00A024580902" 

#include <Streams.h>
#include "CComPtr.h"

#include "win_enumerator.h"
#include "win_webcam_impl.h"
#include "win_webcam_utils.h"

namespace webcam
{
	win_webcam_impl::win_webcam_impl(unsigned char camera_number_, device & webcam_)
		:webcam_impl(camera_number_, webcam_)
	{
		_graph_builder			= nullptr;
		_capture_graph_builder2	= nullptr;
		_media_control			= nullptr;
		_base_filter_source		= nullptr;
		_base_filter_grabber	= nullptr;
		_sample_grabber			= nullptr;
	}

	win_webcam_impl::~win_webcam_impl()
	{
	}

	void win_webcam_impl::open()
	{
		if ((_camera_number < MIN_CAMERA_NUMBER) || (MAX_CAMERA_NUMBER < _camera_number))
		{
			throw_webcam_exception("no such web cam");
		}

		initialize();

		//get interfaces
		_graph_builder = create_graph_builder();

		// Create the capture graph builder
		_capture_graph_builder2 = create_capture_graph_builder2();

		// Obtain interfaces for media control and Video Window
		_media_control = create_media_control();

		_base_filter_source = create_base_filter_source();

		_base_filter_grabber = create_base_filter_grabber();

		_sample_grabber = create_sample_grabber();

		add_filter(_base_filter_source);

		add_filter(_base_filter_grabber);

		apply_video_settings();

		connect_pins();

		set_up_sample_grabber();

		render();

		run();

	}

	void win_webcam_impl::initialize()
	{
		HRESULT res = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
		if (FAILED(res))
		{
			if (res != RPC_E_CHANGED_MODE)
			{
				throw_webcam_exception("can not initialize COM library", res);
			}
		}
	}

	IGraphBuilder * win_webcam_impl::create_graph_builder()
	{
		IGraphBuilder * graph_builder_ = nullptr;
		HRESULT res = CoCreateInstance(CLSID_FilterGraph, nullptr, CLSCTX_INPROC,
			IID_IGraphBuilder, (void **)&graph_builder_);
		if (FAILED(res))
		{
			throw_webcam_exception("can not instaniate IGraphBuilder", res);
		}
		return graph_builder_;
	}

	ICaptureGraphBuilder2 * win_webcam_impl::create_capture_graph_builder2()
	{
		ICaptureGraphBuilder2 * capture_graph_builder2 = nullptr;
		HRESULT res = CoCreateInstance(CLSID_CaptureGraphBuilder2, nullptr, CLSCTX_INPROC,
			IID_ICaptureGraphBuilder2, (void **)&capture_graph_builder2);
		if (FAILED(res))
		{
			throw_webcam_exception("can not instaniate ICaptureGraphBuilder2", res);
		}
		return capture_graph_builder2;
	}

	IMediaControl * win_webcam_impl::create_media_control()
	{
		IMediaControl * media_control = nullptr;
		HRESULT res = _graph_builder->QueryInterface(IID_IMediaControl, (LPVOID *)&media_control);
		if (FAILED(res))
		{
			throw_webcam_exception("can not query interface of IMediaControl", res);
		}
		return media_control;
	}

	IBaseFilter * win_webcam_impl::create_base_filter_source()
	{
		try
		{
			//index is 0 based, number is 1 based
			enumerator enumerator = enumerator::create();
			for (unsigned char camera_index = 0; camera_index < _camera_number; camera_index++)
			{
				device_info_provider provider = enumerator.next();
				if (camera_index + 1 == _camera_number)
				{
					if (provider.is_valid())
					{
						return provider.create_base_filter();
					}
				}
			}
			throw_webcam_exception("no such web cam");
		}
		catch (const video_device_enumerator_exception &){
			throw_webcam_exception("no such web cam");
		}
		return nullptr;
	}

	IBaseFilter * win_webcam_impl::create_base_filter_grabber()
	{
		IBaseFilter * base_filter_grabber = nullptr;
		HRESULT res = CoCreateInstance(CLSID_SampleGrabber, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&base_filter_grabber));
		if (FAILED(res))
		{
			throw_webcam_exception("can not instaniate SampleGrabber", res);
		}
		return base_filter_grabber;
	}

	ISampleGrabber * win_webcam_impl::create_sample_grabber()
	{
		ISampleGrabber * sample_grabber = nullptr;
		HRESULT res = _base_filter_grabber->QueryInterface(IID_PPV_ARGS(&sample_grabber));
		if (FAILED(res))
		{
			throw_webcam_exception("can not query interface of SampleGrabber", res);
		}
		return sample_grabber;
	}

	void win_webcam_impl::add_filter(IBaseFilter * base_filter_)
	{
		HRESULT res = _graph_builder->AddFilter(base_filter_, L"Source");
		if (FAILED(res))
		{
			return throw_webcam_exception("can not add base filter to graph builder", res);
		}
	}

	void win_webcam_impl::apply_traits()
	{
		close();
		open();
	}

	void win_webcam_impl::setup_media_type(AM_MEDIA_TYPE * pmt, const video_settings & video_settings_)
	{
		VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *)pmt->pbFormat;
		unsigned char fps = video_settings_.get_fps();
		pvi->AvgTimePerFrame = (LONGLONG)(10000000 / fps);

		const resolution & resolution = video_settings_.get_resolution();
		pvi->bmiHeader.biWidth = resolution.get_width();
		pvi->bmiHeader.biHeight = resolution.get_height();

		pmt->majortype = MEDIATYPE_Video;
		
		bool is_jpeg = (video_settings_.get_format() == format_JPEG());
		bool is_mjpeg = (video_settings_.get_format() == format_MJPEG());
		if (is_jpeg || is_mjpeg)
		{
			pvi->bmiHeader.biCompression = BI_JPEG;
			pmt->subtype = MEDIASUBTYPE_MJPG;
		}
		/*else
		{
			//pmt->subtype = MEDIASUBTYPE_RGB24;
			pmt->subtype = MEDIASUBTYPE_YUY2;//ok dla kamery wbudowanej
		}*/
	}

	void win_webcam_impl::apply_video_settings()
	{
		const video_settings & video_settings = _device.get_video_settings();
		const resolution & resolution = video_settings.get_resolution();
		const format & format = video_settings.get_format();

		CMediaType mediaType;
		mediaType.SetType(&MEDIATYPE_Video);
		mediaType.SetSubtype(format.get_native_format());

		HRESULT res = _sample_grabber->SetMediaType(&mediaType);
		if (FAILED(res))
		{
			throw_webcam_exception("can not set media type", res);
		}

		// we use this interface to set the frame rate and get the capture size
		CComPtr<IAMStreamConfig> IAM_stream_config;
		res = _capture_graph_builder2->FindInterface
			(&PIN_CATEGORY_CAPTURE,	&MEDIATYPE_Video, _base_filter_source, IID_IAMStreamConfig, (void **)&IAM_stream_config);

		AM_MEDIA_TYPE * pmt = nullptr;
		res = IAM_stream_config->GetFormat(&pmt);
		if ( FAILED(res) )
		{
			throw_webcam_exception("can not get video format", res);
		}
		setup_media_type(pmt, video_settings);
		res = IAM_stream_config->SetFormat(pmt);
		FreeMediaType(*pmt);
		if (FAILED(res))
		{
			throw_webcam_exception("can not set video format", res);
		}
		_grabber.set_video_settings(video_settings);
	}

	void win_webcam_impl::connect_pins()
	{
		// Get the output pin and the input pin
		//
		CComPtr<IPin> source_pin = win_webcam_utils::get_out_pin(_base_filter_source, 0);
		CComPtr<IPin> grabber_pin = win_webcam_utils::get_in_pin(_base_filter_grabber, 0);

		// ... and connect them
		//
		HRESULT res = _graph_builder->Connect(source_pin, grabber_pin);
		if (FAILED(res))
		{
			return throw_webcam_exception("can not connect pins of filters", res);
		}
	}

	void win_webcam_impl::render()
	{
		// Render the grabber output pin (to a video renderer)
		//
		CComPtr<IPin> grab_out_pin = win_webcam_utils::get_out_pin(_base_filter_grabber, 0);
		HRESULT res = _graph_builder->Render(grab_out_pin);
		if (FAILED(res))
		{
			return throw_webcam_exception("can not start rendering", res);
		}
	}

	void win_webcam_impl::set_up_sample_grabber()
	{
		// Don't buffer the samples as they pass through
		HRESULT res = _sample_grabber->SetBufferSamples(FALSE);
		if (FAILED(res))
		{
			return throw_webcam_exception("can not disable buffering", res);
		}
		
		// Only grab one at a time, DO NOT stop stream after
		// grabbing one sample
		res = _sample_grabber->SetOneShot(FALSE);
		if (FAILED(res))
		{
			return throw_webcam_exception("can not enable sampling", res);
		}
		
		res = _sample_grabber->SetCallback(&_grabber, 1);
		if (FAILED(res))
		{
			return throw_webcam_exception("can not set stream callback", res);
		}
	}

	void win_webcam_impl::run()
	{
		CComPtr<IVideoWindow> video_window;
		HRESULT res = _graph_builder->QueryInterface(IID_IVideoWindow, (void **)&video_window);
		if (FAILED(res))
		{
			return throw_webcam_exception("can not query interface of VideoWindow", res);
		}
		
		video_window->put_AutoShow(OAFALSE);
		res = _media_control->Run();
		if (FAILED(res))
		{
			return throw_webcam_exception("can not run MediaControl", res);
		}
	}

	void win_webcam_impl::release_safe()
	{
		if (_media_control)
		{
			HRESULT res = _media_control->Stop();
			_media_control->Release();
			_media_control = nullptr;
		}

		if (_sample_grabber)
		{
			_sample_grabber->Release();
			_sample_grabber = nullptr;
		}

		if (_base_filter_grabber)
		{
			_base_filter_grabber->Release();
			_base_filter_grabber = nullptr;
		}

		if (_base_filter_source)
		{
			_base_filter_source->Release();
			_base_filter_source = nullptr;
		}

		if (_capture_graph_builder2)
		{
			_capture_graph_builder2->Release();
			_capture_graph_builder2 = nullptr;
		}

		if (_graph_builder)
		{
			_graph_builder->Release();
			_graph_builder = nullptr;
		}

		CoUninitialize();
	}

	void win_webcam_impl::close()
	{
		release_safe();
	}

	void win_webcam_impl::throw_webcam_exception(const char * what_, int err_no_)
	{
		throw webcam_exception(what_, err_no_, _camera_number);
	}

	image * win_webcam_impl::read()
	{
		return _grabber.grab();
	}
}

#endif
