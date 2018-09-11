/*
 * device.cpp
 *
 *  Created on: Feb 11, 2013
 *      Author: zebul
 */

#include "Device.h"
#include "state/closed.h"

#ifdef WIN32
	#include "os/win/win_webcam_impl.h"
#define WEBCAM_IMPL win_webcam_impl
#else
	#include "os/lin/lin_webcam_impl.h"
#define WEBCAM_IMPL lin_webcam_impl
#endif

namespace webcam
{

	Device::Device(unsigned char number_, const VideoSettings & video_settings_)
		: _state( closed::instance() )
		, _video_settings(video_settings_)
	{
		_impl = new WEBCAM_IMPL(number_, *this);
	}

	Device::~Device() {
		
		_state->release_safe(*this);
		delete _impl;
		delete _state;
	}

	void Device::set_state(state  * state_)
	{
		_state = state_;
	}

	void Device::open()
	{
		_state->open(*this);
	}

	void Device::close()
	{
		_state->close(*this);
	}

	Image * Device::read()
	{
		return _state->read(*this);
	}

	bool Device::is_open()const
	{
		return _state->is_open();
	}

	unsigned char Device::get_number()const
	{
		return _impl->get_webcam_number();
	}

	const VideoSettings & Device::get_video_settings()const
	{
		return _video_settings;
	}

	void Device::set_video_settings(const VideoSettings & video_settings_)
	{
		_state->set_video_settings(*this, video_settings_);
	}

	bool Device::is_resolution_adjustable()const
	{
		return true;
	}

	bool Device::is_quality_adjustable()const
	{
		return true;
	}

	void Device::do_set_video_settings(const VideoSettings & video_settings_)
	{
		_video_settings = video_settings_;
	}

}
