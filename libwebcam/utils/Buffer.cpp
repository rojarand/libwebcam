#include "Buffer.h"
#include <cstring>

namespace webcam
{

	BufferData::BufferData(unsigned char * data_, unsigned int lenght_)
		:_data(data_)
		, _lenght(lenght_)
	{

	}

	unsigned char * BufferData::get_data()
	{
		return _data;
	}
	
	unsigned int BufferData::get_lenght()const
	{
		return _lenght;
	}

	
	BufferData::BufferData()
		:_data(nullptr)
		, _lenght(0)
	{

	}

	Buffer::Buffer()
	{

	}

	Buffer::Buffer(const unsigned char * buf_, unsigned int len_)
	{
		alloc(buf_, len_);
	}

	Buffer::Buffer(const Buffer & buffer_)
	{
		alloc(buffer_._buffer_data._data, buffer_._buffer_data._lenght);
	}

	Buffer::~Buffer()
	{
		free();
	}

	void Buffer::attach(const BufferData & buffer_data_)
	{
		free();
		_buffer_data = buffer_data_;
	}

	BufferData Buffer::detach()
	{
		BufferData data = _buffer_data;
		_buffer_data._lenght = 0;
		_buffer_data._data = nullptr;
		return data;
	}

	const unsigned char * Buffer::get_data()const
	{
		return _buffer_data._data;
	}
	unsigned char * Buffer::get_data()
	{
		return _buffer_data._data;
	}

	unsigned int Buffer::get_length()const
	{
		return _buffer_data._lenght;
	}

	void Buffer::alloc(const unsigned char * data_, unsigned int lenght_)
	{
		free();
		if (0 < lenght_)
		{
			_buffer_data._data = new unsigned char[lenght_];
			_buffer_data._lenght = lenght_;
			memcpy(_buffer_data._data, data_, _buffer_data._lenght);
		}
	}

	void Buffer::free()
	{
		if(_buffer_data._data)
		{
			delete[] _buffer_data._data;
			_buffer_data._data = nullptr;
		}
		_buffer_data._lenght = 0;
	}

	bool Buffer::is_empty()const
	{
		return (_buffer_data._lenght == 0);
	}

	unsigned char & Buffer::operator[](const int index_)
	{
		return _buffer_data._data[index_];
	}

	Buffer & Buffer::operator=(const Buffer & buffer_)
	{
		alloc(buffer_._buffer_data._data, buffer_._buffer_data._lenght);
		return *this;
	}
}