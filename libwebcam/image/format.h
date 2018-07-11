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

	class Format
	{
	public:
		virtual ~Format();
		virtual std::string get_name()const;
		virtual native_format get_native_format()const = 0;
		virtual Format * clone()const = 0;
		bool operator==(const Format & format_)const;
		bool native_equals(const native_format & native_format_)const;

	};

	class Format_UNKN : public Format
	{
	public:
		Format_UNKN( const native_format& native_format_ );

		virtual native_format get_native_format()const;
		virtual Format * clone()const;
		native_format format;
	};

	class Format_BGR8 : public Format
	{
	public:
        virtual std::string get_name()const;
		virtual native_format get_native_format()const;
		virtual Format * clone()const;
	};

	class Format_BMP24 : public Format
	{
	public:
		virtual native_format get_native_format()const;
		virtual Format * clone()const;
	};

	class Format_RGB24 : public Format
	{
	public:
        virtual std::string get_name()const;
		virtual native_format get_native_format()const;
		virtual Format * clone()const;
	};

	class Format_I420 : public Format
	{
	public:
        virtual std::string get_name()const;
		virtual native_format get_native_format()const;
		virtual Format * clone()const;
	};

	class Format_YUY2 : public Format
	{
	public:
        virtual std::string get_name()const;
		virtual native_format get_native_format()const;
		virtual Format * clone()const;
	};

	class Format_YUYV : public Format
	{
	public:
		virtual native_format get_native_format()const;
		virtual Format * clone()const;
	};

	class Format_H264 : public Format
	{
	public:
		virtual native_format get_native_format()const;
		virtual Format * clone()const;
	};

	class Format_JPEG: public Format
	{
	public:
		virtual native_format get_native_format()const;
		virtual Format * clone()const;
	};

	class Format_MJPEG : public Format
	{
	public:
		virtual native_format get_native_format()const;
		virtual Format * clone()const;
	};

    Format * create_format(native_format native_format_);

}
