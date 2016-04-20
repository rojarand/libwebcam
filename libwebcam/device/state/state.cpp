#include "state.h"
#include "../../error/webcam_exception.h"

namespace webcam
{

	state::state()
	{
	}


	state::~state(void)
	{
	}


	void state::set_state(device & device_, state * state_)
	{
		device_.set_state(state_);
	}

	webcam_impl & state::get_impl(device & device_)
	{
		return (*device_._impl);
	}

	void state::set_video_settings(device & device_, const video_settings & video_settings_)
	{
		device_.do_set_video_settings(video_settings_);
	}

	void state::open(device & device_)
	{
		throw webcam_exception("device already open");
	}

	void state::close(device & device_)
	{
		throw webcam_exception("device already closed");
	}
	
	image * state::read(device & device_)
	{
		throw webcam_exception("can not read (device not open)");
	}

	bool state::is_open()const
	{
		return false;
	}
}
