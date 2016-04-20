#pragma once

#include <string>

namespace webcam
{

	class nameable
	{
	public:
		nameable(const std::string & name_);
		~nameable();
		const std::string & get_name()const;
		void set_name(const std::string & name_);
	protected:
		std::string _name;
	};

}