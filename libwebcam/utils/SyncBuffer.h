#pragma once

#include <mutex>

#include "Buffer.h"

namespace webcam {

class SyncBuffer
{
public:
	SyncBuffer(void);
	SyncBuffer(const unsigned char * buf_, unsigned int len_);
	~SyncBuffer(void);
	void free();
	void alloc(const unsigned char * buf_, unsigned int len_);
	void attach(const BufferData & buffer_data_);
	BufferData detach();
protected:
	webcam::Buffer buffer;
	std::mutex mutex;
};

}
