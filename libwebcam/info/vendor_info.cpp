#include "vendor_info.h"

namespace webcam
{
	const int vendor_info::UNKNOWN_ID = 0;

	vendor_info::vendor_info(int id_, const std::string & name_)
		: basic_info(id_, name_)
	{
	}

	vendor_info::vendor_info()
		: basic_info(UNKNOWN_ID, "")
	{
	}

	vendor_info::~vendor_info()
	{
	}

	bool vendor_info::is_known()const
	{
		return _id != UNKNOWN_ID;
	}
}