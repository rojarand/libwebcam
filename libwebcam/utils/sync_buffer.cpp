#include "sync_buffer.h"

#ifndef WIN32
	#include <cstring>
#endif

namespace webcam {

sync_buffer::sync_buffer(void)
{
}

sync_buffer::sync_buffer(const unsigned char * buf_, unsigned int len_)
{
	std::lock_guard<std::mutex> lock(mutex);
	buffer.alloc(buf_, len_);
}

sync_buffer::~sync_buffer(void)
{
}


void sync_buffer::alloc(const unsigned char * buf_, unsigned int len_)
{
	std::lock_guard<std::mutex> lock(mutex);
	buffer.alloc(buf_, len_);
}

void sync_buffer::free()
{
	std::lock_guard<std::mutex> lock(mutex);
	buffer.free();
}

void sync_buffer::attach(const buffer_data & buffer_data_)
{
	std::lock_guard<std::mutex> lock(mutex);
	buffer.attach(buffer_data_);
}

buffer_data sync_buffer::detach()
{
	std::lock_guard<std::mutex> lock(mutex);
	return buffer.detach();
}

}
