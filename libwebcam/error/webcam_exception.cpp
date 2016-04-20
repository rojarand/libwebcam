#include "webcam_exception.h"
#include <iostream>
#include <sstream>

#ifndef WIN32
#include <cstring>
#endif

namespace webcam
{

	webcam_exception::webcam_exception(const char * what_, int error_number_, unsigned char webcam_number_)
		: std::runtime_error(what_)
		, _error_number(error_number_)
		, _webcam_number(webcam_number_)
	{
	}

	webcam_exception::~webcam_exception()throw(){
		
	}

	int webcam_exception::get_error_number() const throw()
	{
		return _error_number;
	}

	unsigned char webcam_exception::get_webcam_number()const
	{
		return _webcam_number;
	}

	std::string webcam_exception::get_details()const
	{
		if (SUCCESS == _error_number)
		{
			return "";
		}

#ifdef WIN32
		_com_error err((HRESULT)_error_number);
		std::wstring wmessage = err.ErrorMessage();
		std::string message(wmessage.begin(), wmessage.end());
		return message;
#else
		std::string message = strerror(_error_number);
		return message;

#endif
	}
}
