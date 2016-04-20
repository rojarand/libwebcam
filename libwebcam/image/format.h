#pragma once

#include <string>

#ifdef WIN32
#include <wmsdkidl.h>
#include <Uuids.h>
EXTERN_GUID(MEDIASUBTYPE_I420, 0x30323449, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71);
EXTERN_GUID(MEDIASUBTYPE_UNKN, 0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
#endif

namespace webcam
{

#ifdef WIN32
	typedef const GUID * native_format;
#else
	typedef unsigned int native_format;
#endif

	class format
	{
	public:
		virtual ~format();
		std::string get_name()const;
		virtual native_format get_native_format()const = 0;
		virtual format * clone()const = 0;
		bool operator==(const format & format_)const;
		bool native_equals(const native_format & native_format_)const;
	};

	class format_UNKN : public format
	{
	public:
		virtual native_format get_native_format()const;
		virtual format * clone()const;
	};

	class format_BGR8 : public format
	{
	public:
		virtual native_format get_native_format()const;
		virtual format * clone()const;
	};

	class format_BMP24 : public format
	{
	public:
		virtual native_format get_native_format()const;
		virtual format * clone()const;
	};

	class format_RGB24 : public format
	{
	public:
		virtual native_format get_native_format()const;
		virtual format * clone()const;
	};

	class format_I420 : public format
	{
	public:
		virtual native_format get_native_format()const;
		virtual format * clone()const;
	};

	class format_YUY2 : public format
	{
	public:
		virtual native_format get_native_format()const;
		virtual format * clone()const;
	};

	class format_YUYV : public format
	{
	public:
		virtual native_format get_native_format()const;
		virtual format * clone()const;
	};

	class format_H264 : public format
	{
	public:
		virtual native_format get_native_format()const;
		virtual format * clone()const;
	};

	class format_JPEG: public format
	{
	public:
		virtual native_format get_native_format()const;
		virtual format * clone()const;
	};

	class format_MJPEG : public format
	{
	public:
		virtual native_format get_native_format()const;
		virtual format * clone()const;
	};

	format * create_format(native_format native_format_);

}
