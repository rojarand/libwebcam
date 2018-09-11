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


	void state::set_state(Device & device_, state * state_)
	{
		device_.set_state(state_);
	}

	WebcamImpl & state::get_impl(Device & device_)
	{
		return (*device_._impl);
	}

	void state::set_video_settings(Device & device_, const VideoSettings & video_settings_)
	{
		device_.do_set_video_settings(video_settings_);
	}

	void state::open(Device & device_)
	{
		throw webcam_exception("device already open");
	}

	void state::close(Device & device_)
	{
		throw webcam_exception("device already closed");
	}
	
	Image * state::read(Device & device_)
	{
		throw webcam_exception("can not read (device not open)");
	}

	bool state::is_open()const
	{
		return false;
	}
}
