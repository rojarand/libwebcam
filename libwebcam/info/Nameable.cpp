#include "Nameable.h"

namespace webcam
{

	Nameable::Nameable(const std::string & name_)
		:_name(name_)
	{
	}


	Nameable::~Nameable()
	{
	}

	const std::string & Nameable::get_name()const
	{
		return _name;
	}

	void Nameable::set_name(const std::string & name_)
	{
		_name = name_;
	}

}