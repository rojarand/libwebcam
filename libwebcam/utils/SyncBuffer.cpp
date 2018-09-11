#include "SyncBuffer.h"

#ifndef WIN32
	#include <cstring>
#endif

namespace webcam {

SyncBuffer::SyncBuffer(void)
{
}

SyncBuffer::SyncBuffer(const unsigned char * buf_, unsigned int len_)
{
	std::lock_guard<std::mutex> lock(mutex);
	buffer.alloc(buf_, len_);
}

SyncBuffer::~SyncBuffer(void)
{
}


void SyncBuffer::alloc(const unsigned char * buf_, unsigned int len_)
{
	std::lock_guard<std::mutex> lock(mutex);
	buffer.alloc(buf_, len_);
}

void SyncBuffer::free()
{
	std::lock_guard<std::mutex> lock(mutex);
	buffer.free();
}

void SyncBuffer::attach(const BufferData & buffer_data_)
{
	std::lock_guard<std::mutex> lock(mutex);
	buffer.attach(buffer_data_);
}

BufferData SyncBuffer::detach()
{
	std::lock_guard<std::mutex> lock(mutex);
	return buffer.detach();
}

}
