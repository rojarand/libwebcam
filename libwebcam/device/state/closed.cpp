#include "closed.h"
#include "open.h"

namespace webcam
{

	closed::closed(void)
	{
	}

	closed::~closed(void)
	{
	}

	void closed::open(Device & device_)
	{
		try
		{
			get_impl(device_).open();
			set_state(device_, _open::instance());
			delete this;
		}
		catch (const webcam_exception & exc_)
		{
			get_impl(device_).release_safe();
			throw exc_;
		}
	}

	closed * closed::instance()
	{
		return new closed();
	}
}
