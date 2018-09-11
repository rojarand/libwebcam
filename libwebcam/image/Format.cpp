#include "Format.h"

namespace webcam
{

	Format::~Format()
	{

	}
	
	bool Format::operator == (const Format & format_)const
	{
		return native_equals(format_.get_native_format());
	}

	Format_UNKN::Format_UNKN( const native_format& native_format_ ) : format(native_format_)
    {
	}

	native_format Format_UNKN::get_native_format() const
	{
		return this->format;
	}

	Format * Format_UNKN::clone()const
	{
		return new Format_UNKN(this->format);
	}

	Format * Format_BGR8::clone()const
	{
		return new Format_BGR8();
	}

	Format * Format_BMP24::clone()const
	{
		return new Format_BMP24();
	}

	Format * Format_RGB24::clone()const
	{
		return new Format_RGB24();
	}

	Format * Format_I420::clone()const
	{
		return new Format_I420();
	}

	Format * Format_YUY2::clone()const
	{
		return new Format_YUY2();
	}

	Format * Format_YUYV::clone()const
	{
		return new Format_YUYV();
	}

	Format * Format_H264::clone()const
	{
		return new Format_H264();
	}

	Format * Format_JPEG::clone()const
	{
		return new Format_JPEG();
	}

	Format * Format_MJPEG::clone()const
	{
		return new Format_MJPEG();
	}

	Format * create_format(native_format native_format_)
	{
		const Format_MJPEG FORMAT_MJPEG;
		const Format_JPEG FORMAT_JPEG;
		const Format_I420 FORMAT_I420;
		const Format_YUYV FORMAT_YUYV;
		const Format_YUY2 FORMAT_YUY2;
		const Format_BMP24 FORMAT_BMP24;
		const Format_RGB24 FORMAT_RGB24;
		const Format_BGR8 FORMAT_BGR8;

		const Format * const SUPPORTED_FORMATS[] = {
				&FORMAT_MJPEG, &FORMAT_JPEG, &FORMAT_I420, &FORMAT_YUYV,
				&FORMAT_YUY2, &FORMAT_BMP24, &FORMAT_RGB24, &FORMAT_BGR8 };

		for(const Format * fmt: SUPPORTED_FORMATS){

			if (fmt->native_equals(native_format_)){
				return fmt->clone();
			}
		}
		return new Format_UNKN(native_format_);
	}
}
