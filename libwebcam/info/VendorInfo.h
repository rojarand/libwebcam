#pragma once

#include "BasicInfo.h"

namespace webcam
{
	class VendorInfo : public BasicInfo
	{
	public:
		static const int UNKNOWN_ID;
		VendorInfo(int id_, const std::string & name_);
		VendorInfo();
		~VendorInfo();
		bool is_known()const;
	};
}