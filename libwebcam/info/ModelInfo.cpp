#include "ModelInfo.h"

namespace webcam
{
	const int ModelInfo::UNKNOWN_ID = 0;

	ModelInfo::ModelInfo()
		: BasicInfo(UNKNOWN_ID, "")
	{
	}

	ModelInfo::ModelInfo(int id_, const std::string & name_)
		: BasicInfo(id_, name_)
	{
	}

	ModelInfo::~ModelInfo()
	{
	}

	const std::string & ModelInfo::get_desc()const
	{
		return _desc;
	}

	void ModelInfo::set_desc(const std::string & desc_)
	{
		_desc = desc_;
	}

	const std::string & ModelInfo::get_friendly_name()const
	{
		return _friendly_name;
	}

	void ModelInfo::set_friendly_name(const std::string & friendly_name_)
	{
		_friendly_name = friendly_name_;
	}

	const VendorInfo & ModelInfo::get_vendor_info()const
	{
		return _vendor_info;
	}

	void ModelInfo::set_vendor_info(const VendorInfo & vendor_info_)
	{
		_vendor_info = vendor_info_;
	}
}