#pragma once

#include <string>

namespace webcam
{

	class Nameable
	{
	public:
		Nameable(const std::string & name_);
		~Nameable();
		const std::string & get_name()const;
		void set_name(const std::string & name_);
	protected:
		std::string _name;
	};

}