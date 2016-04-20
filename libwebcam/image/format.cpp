#include "format.h"

namespace webcam
{

	format::~format()
	{

	}
	
	bool format::operator == (const format & format_)const
	{
		return native_equals(format_.get_native_format());
	}

	format * format_UNKN::clone()const
	{
		return new format_UNKN();
	}

	format * format_BGR8::clone()const
	{
		return new format_BGR8();
	}

	format * format_BMP24::clone()const
	{
		return new format_BMP24();
	}

	format * format_RGB24::clone()const
	{
		return new format_RGB24();
	}

	format * format_I420::clone()const
	{
		return new format_I420();
	}

	format * format_YUY2::clone()const
	{
		return new format_YUY2();
	}

	format * format_YUYV::clone()const
	{
		return new format_YUYV();
	}

	format * format_H264::clone()const
	{
		return new format_H264();
	}

	format * format_JPEG::clone()const
	{
		return new format_JPEG();
	}

	format * format_MJPEG::clone()const
	{
		return new format_MJPEG();
	}

	format * create_format(native_format native_format_)
	{
		const format_MJPEG FORMAT_MJPEG;
		const format_JPEG FORMAT_JPEG;
		const format_I420 FORMAT_I420;
		const format_YUYV FORMAT_YUYV;
		const format_YUY2 FORMAT_YUY2;
		const format_BMP24 FORMAT_BMP24;
		const format_RGB24 FORMAT_RGB24;
		const format_BGR8 FORMAT_BGR8;

		const format * const SUPPORTED_FORMATS[] = {
				&FORMAT_MJPEG, &FORMAT_JPEG, &FORMAT_I420, &FORMAT_YUYV,
				&FORMAT_YUY2, &FORMAT_BMP24, &FORMAT_RGB24, &FORMAT_BGR8 };

		for(const format * fmt: SUPPORTED_FORMATS){

			if (fmt->native_equals(native_format_)){
				return fmt->clone();
			}
		}
		return new format_UNKN();
	}
}
