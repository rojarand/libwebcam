#pragma once

#include "Indexable.h"
#include "Nameable.h"

namespace webcam
{

	class BasicInfo : public Indexable, public Nameable
	{
	public:
		BasicInfo(int id_, const std::string & name_);
		~BasicInfo();
	};

}