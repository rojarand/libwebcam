#include "open.h"
#include "closed.h"

namespace webcam
{

	_open::_open(void)
	{
	}


	_open::~_open(void)
	{
	}

	void _open::close(Device & device_)
	{
		try
		{
			get_impl(device_).close();
			set_state(device_, closed::instance());
			delete this;
		}
		catch (const webcam_exception & exc_)
		{
			set_state(device_, closed::instance());
			delete this;
			throw exc_;
		}
	}

	_open * _open::instance()
	{
		return new _open();
	}

	Image * _open::read(Device & device_)
	{
		try
		{
			return get_impl(device_).read();
		}
		catch (const webcam_exception & exc_)
		{
			get_impl(device_).release_safe();
			set_state(device_, closed::instance());
			delete this;
			throw exc_;
		}
	}

	void _open::set_video_settings(Device & device_, const VideoSettings & video_settings_)
	{
		try
		{
			get_impl(device_).close();
			state::set_video_settings(device_, video_settings_);
			get_impl(device_).open();
		}
		catch (const webcam_exception & exc_)
		{
			get_impl(device_).release_safe();
			set_state(device_, closed::instance());
			delete this;
			throw exc_;
		}
	}

	bool _open::is_open()const
	{
		return true;
	}

	void _open::release_safe(Device & device_)
	{
		get_impl(device_).release_safe();
	}
}
