#include "../../format.h"

#ifdef WIN32
#include <Fourcc.h>

namespace webcam
{
	bool format::native_equals(const native_format & native_format_)const{

		return (*get_native_format()) == (*native_format_);
	}

	std::string format::get_name()const
	{
		native_format format = get_native_format();
		if(format == nullptr)
		{
			return "";
		}
		
		FOURCCMap fccMap(format);
		DWORD four_cc = fccMap.GetFOURCC();
		
		char four_cc_str[5] =
		{
			((four_cc >> 0)	& 0xFF),
			((four_cc >> 8) & 0xFF),
			((four_cc >> 16)& 0xFF),
			((four_cc >> 24)& 0xFF),
			0
		};
		return four_cc_str;
	}

	native_format format_UNKN::get_native_format()const
	{
		return &MEDIASUBTYPE_UNKN;
	}

	native_format format_BGR8::get_native_format()const
	{
		return &MEDIASUBTYPE_RGB8;
	}

	native_format format_BMP24::get_native_format()const
	{
		return &MEDIASUBTYPE_RGB24;
	}

	native_format format_RGB24::get_native_format()const
	{
		return &MEDIASUBTYPE_UNKN;
	}

	native_format format_I420::get_native_format()const
	{
		return &MEDIASUBTYPE_I420;
	}

	native_format format_YUY2::get_native_format()const
	{
		return &MEDIASUBTYPE_YUY2;
	}

	native_format format_YUYV::get_native_format()const
	{
		return &MEDIASUBTYPE_YUYV;
	}

	native_format format_H264::get_native_format()const
	{
		return &MEDIASUBTYPE_H264;
	}

	native_format format_JPEG::get_native_format()const
	{
		return &MEDIASUBTYPE_MJPG;
	}

	native_format format_MJPEG::get_native_format()const
	{
		return &MEDIASUBTYPE_MJPG;
	}
}

#endif
