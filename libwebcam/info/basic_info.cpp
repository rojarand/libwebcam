#include "basic_info.h"


namespace webcam
{

	basic_info::basic_info(int id_, const std::string & name_)
		: indexable(id_)
		, nameable(name_)
	{
	}


	basic_info::~basic_info()
	{
	}

}