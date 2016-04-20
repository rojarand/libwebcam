#include "nameable.h"

namespace webcam
{

	nameable::nameable(const std::string & name_)
		:_name(name_)
	{
	}


	nameable::~nameable()
	{
	}

	const std::string & nameable::get_name()const
	{
		return _name;
	}

	void nameable::set_name(const std::string & name_)
	{
		_name = name_;
	}

}