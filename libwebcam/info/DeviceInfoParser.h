#pragma once

#include "DeviceInfo.h"
#include "VendorModelIndices.h"

namespace webcam
{

	class DeviceInfoParser
	{
	public:
		static ModelInfo provide_model_info(const VendorModelIndices & vendor_model_indices_, 
			std::istream & vendor_product_stream_);
	private:
		static bool is_vendor_line(const std::string & line_);
		static bool is_product_line(const std::string & line_);
		static bool is_comment_line(const std::string & line_);
		static bool is_empty_line(const std::string & line_);
		static BasicInfo build_basic_info(const std::string & line_);
	};

}