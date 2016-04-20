#pragma once

#include "indexable.h"
#include "nameable.h"

namespace webcam
{

	class basic_info : public indexable, public nameable
	{
	public:
		basic_info(int id_, const std::string & name_);
		~basic_info();
	};

}