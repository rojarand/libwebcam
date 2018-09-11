#include "VendorInfo.h"

namespace webcam
{
	const int VendorInfo::UNKNOWN_ID = 0;

	VendorInfo::VendorInfo(int id_, const std::string & name_)
		: BasicInfo(id_, name_)
	{
	}

	VendorInfo::VendorInfo()
		: BasicInfo(UNKNOWN_ID, "")
	{
	}

	VendorInfo::~VendorInfo()
	{
	}

	bool VendorInfo::is_known()const
	{
		return _id != UNKNOWN_ID;
	}
}