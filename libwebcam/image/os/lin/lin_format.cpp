#include "../../format.h"

#ifndef WIN32

#include <linux/videodev2.h>

namespace webcam
{

	bool format::native_equals(const native_format & native_format_)const{

		return get_native_format() == native_format_;
	}

	std::string format::get_name()const
	{
		native_format format = get_native_format();
		if(format == 0)
		{
			return "UNKNOWN_0";
		}

		char four_cc_str[5] =
		{
			static_cast<char>((format >> 0) & 0xFF),
			static_cast<char>((format >> 8) & 0xFF),
			static_cast<char>((format >> 16)& 0xFF),
			static_cast<char>((format >> 24)& 0xFF),
			0
		};
		return four_cc_str;
	}

	native_format format_UNKN::get_native_format()const
	{
		return 0;
	}
	std::string format_UNKN::get_name()const
	{
		return "UNKN";
	}

	native_format format_BGR8::get_native_format()const
	{
		return 0;
	}
	std::string format_BGR8::get_name()const
	{
		return "BGR8";
	}

	native_format format_RGB24::get_native_format()const
	{
		return V4L2_PIX_FMT_RGB24;
	}
	std::string format_RGB24::get_name()const
	{
		return "RGB24";
	}

	native_format format_BMP24::get_native_format()const
	{
		return V4L2_PIX_FMT_BGR24;
	}

	native_format format_I420::get_native_format()const
	{
		return 0;
	}
	std::string format_I420::get_name()const
	{
		return "I420";
	}

	native_format format_YUY2::get_native_format()const
	{
		return 0;
	}
	std::string format_YUY2::get_name()const
	{
		return "YUY2";
	}

	native_format format_YUYV::get_native_format()const
	{
		return V4L2_PIX_FMT_YUYV;
	}

	native_format format_H264::get_native_format()const
	{
		return V4L2_PIX_FMT_H264;
	}

	native_format format_JPEG::get_native_format()const
	{
		return V4L2_PIX_FMT_JPEG;
	}

	native_format format_MJPEG::get_native_format()const
	{
		return V4L2_PIX_FMT_MJPEG;
	}
}

#endif
