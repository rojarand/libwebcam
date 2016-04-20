#pragma once

#include <mutex>

#include "buffer.h"

namespace webcam {

class sync_buffer
{
public:
	sync_buffer(void);
	sync_buffer(const unsigned char * buf_, unsigned int len_);
	~sync_buffer(void);
	void free();
	void alloc(const unsigned char * buf_, unsigned int len_);
	void attach(const buffer_data & buffer_data_);
	buffer_data detach();
protected:
	webcam::buffer buffer;
	std::mutex mutex;
};

}
