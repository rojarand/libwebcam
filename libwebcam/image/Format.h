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
	typedef unsigned int native_format;

	int lookup_format_four_cc(int native_format_);

	std::string lookup_format(int native_format_);

	int lookup_format( const std::string& format );
}
