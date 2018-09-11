#include "BasicInfo.h"


namespace webcam
{

	BasicInfo::BasicInfo(int id_, const std::string & name_)
		: Indexable(id_)
		, Nameable(name_)
	{
	}


	BasicInfo::~BasicInfo()
	{
	}

}