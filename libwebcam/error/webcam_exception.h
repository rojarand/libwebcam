#pragma once

#include <stdexcept>
#include <string>

#ifdef WIN32
#include <comdef.h>
#endif

namespace webcam
{

	enum
	{
		INVALID_CAMERA_NUMBER = 0,
#ifdef WIN32
		SUCCESS = S_OK,
#else
		FAILURE = -1,
		SUCCESS = 0,

#endif
	};

	class webcam_exception : public std::runtime_error
	{
	public:
		webcam_exception(const char * what_, int error_number_ = SUCCESS,
			unsigned char webcam_number_ = INVALID_CAMERA_NUMBER);
		virtual ~webcam_exception()throw();

		std::string get_details()const;
		int get_error_number() const throw();
		unsigned char get_webcam_number()const;
	protected:
		int _error_number;
		unsigned char _webcam_number;
	};

}
