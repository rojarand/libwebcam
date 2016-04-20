#pragma once

#include "device_info.h"
#include "vendor_model_indices.h"

namespace webcam
{

	class device_info_parser
	{
	public:
		static model_info provide_model_info(const vendor_model_indices & vendor_model_indices_, 
			std::istream & vendor_product_stream_);
	private:
		static bool is_vendor_line(const std::string & line_);
		static bool is_product_line(const std::string & line_);
		static bool is_comment_line(const std::string & line_);
		static bool is_empty_line(const std::string & line_);
		static basic_info build_basic_info(const std::string & line_);
	};

}