#include <sstream>
#include "text_utils.h"

namespace webcam
{
	text_utils::text_utils()
	{
	}


	int text_utils::hex_str_2_dec_int(const std::string & hex_str_)
	{
		std::stringstream stream;
		stream << std::hex << hex_str_;
		int dec_int = 0;
		stream >> dec_int;
		return dec_int;
	}
}