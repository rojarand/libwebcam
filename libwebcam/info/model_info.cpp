#include "model_info.h"

namespace webcam
{
	const int model_info::UNKNOWN_ID = 0;

	model_info::model_info()
		: basic_info(UNKNOWN_ID, "")
	{
	}

	model_info::model_info(int id_, const std::string & name_)
		: basic_info(id_, name_)
	{
	}

	model_info::~model_info()
	{
	}

	const std::string & model_info::get_desc()const
	{
		return _desc;
	}

	void model_info::set_desc(const std::string & desc_)
	{
		_desc = desc_;
	}

	const std::string & model_info::get_friendly_name()const
	{
		return _friendly_name;
	}

	void model_info::set_friendly_name(const std::string & friendly_name_)
	{
		_friendly_name = friendly_name_;
	}

	const vendor_info & model_info::get_vendor_info()const
	{
		return _vendor_info;
	}

	void model_info::set_vendor_info(const vendor_info & vendor_info_)
	{
		_vendor_info = vendor_info_;
	}
}