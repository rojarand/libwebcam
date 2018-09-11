#pragma once

#include <string>
#include "VendorInfo.h"

#include "BasicInfo.h"

namespace webcam
{
	class ModelInfo : public BasicInfo
	{
	public:
		static const int UNKNOWN_ID;
		ModelInfo();
		ModelInfo(int id_, const std::string & name_);
		~ModelInfo();

		const std::string & get_desc()const;
		void set_desc(const std::string & desc_);

		const std::string & get_friendly_name()const;
		void set_friendly_name(const std::string & friendly_name_);

		const VendorInfo & get_vendor_info()const;
		void set_vendor_info(const VendorInfo & vendor_info_);
	private:
		std::string _desc;
		std::string _friendly_name;
		VendorInfo _vendor_info;
	};
}
