/*
 * device.cpp
 *
 *  Created on: Feb 11, 2013
 *      Author: zebul
 */

#include "device.h"
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

	device::device(unsigned char number_, const video_settings & video_settings_)
		: _state( closed::instance() )
		, _video_settings(video_settings_)
	{
		_impl = new WEBCAM_IMPL(number_, *this);
	}

	device::~device() {
		
		_state->release_safe(*this);
		delete _impl;
		delete _state;
	}

	void device::set_state(state  * state_)
	{
		_state = state_;
	}

	void device::open()
	{
		_state->open(*this);
	}

	void device::close()
	{
		_state->close(*this);
	}

	image * device::read()
	{
		return _state->read(*this);
	}

	bool device::is_open()const
	{
		return _state->is_open();
	}

	unsigned char device::get_number()const
	{
		return _impl->get_webcam_number();
	}

	const video_settings & device::get_video_settings()const
	{
		return _video_settings;
	}

	void device::set_video_settings(const video_settings & video_settings_)
	{
		_state->set_video_settings(*this, video_settings_);
	}

	bool device::is_resolution_adjustable()const
	{
		return true;
	}

	bool device::is_quality_adjustable()const
	{
		return true;
	}

	void device::do_set_video_settings(const video_settings & video_settings_)
	{
		_video_settings = video_settings_;
	}

}
