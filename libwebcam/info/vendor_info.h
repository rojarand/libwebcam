#pragma once

#include "basic_info.h"

namespace webcam
{
	class vendor_info : public basic_info
	{
	public:
		static const int UNKNOWN_ID;
		vendor_info(int id_, const std::string & name_);
		vendor_info();
		~vendor_info();
		bool is_known()const;
	};
}