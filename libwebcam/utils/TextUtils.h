#pragma once

#include <string>

namespace webcam
{

	class TextUtils
	{
	public:
		static int hex_str_2_dec_int(const std::string & hex_str_);
	private:
		TextUtils();
	};

}