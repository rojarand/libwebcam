#pragma once

#include <string>

namespace webcam
{

	class text_utils
	{
	public:
		static int hex_str_2_dec_int(const std::string & hex_str_);
	private:
		text_utils();
	};

}