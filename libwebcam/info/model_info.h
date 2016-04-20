#pragma once

#include <string>
#include "vendor_info.h"

#include "basic_info.h"

namespace webcam
{
	class model_info : public basic_info
	{
	public:
		static const int UNKNOWN_ID;
		model_info();
		model_info(int id_, const std::string & name_);
		~model_info();

		const std::string & get_desc()const;
		void set_desc(const std::string & desc_);

		const std::string & get_friendly_name()const;
		void set_friendly_name(const std::string & friendly_name_);

		const vendor_info & get_vendor_info()const;
		void set_vendor_info(const vendor_info & vendor_info_);
	private:
		std::string _desc;
		std::string _friendly_name;
		vendor_info _vendor_info;
	};
}
