#include "buffer.h"
#include <cstring>

namespace webcam
{

	buffer_data::buffer_data(unsigned char * data_, unsigned int lenght_)
		:_data(data_)
		, _lenght(lenght_)
	{

	}

	unsigned char * buffer_data::get_data()
	{
		return _data;
	}
	
	unsigned int buffer_data::get_lenght()const
	{
		return _lenght;
	}

	
	buffer_data::buffer_data()
		:_data(nullptr)
		, _lenght(0)
	{

	}

	buffer::buffer()
	{

	}

	buffer::buffer(const unsigned char * buf_, unsigned int len_)
	{
		alloc(buf_, len_);
	}

	buffer::buffer(const buffer & buffer_)
	{
		alloc(buffer_._buffer_data._data, buffer_._buffer_data._lenght);
	}

	buffer::~buffer()
	{
		free();
	}

	void buffer::attach(const buffer_data & buffer_data_)
	{
		free();
		_buffer_data = buffer_data_;
	}

	buffer_data buffer::detach()
	{
		buffer_data data = _buffer_data;
		_buffer_data._lenght = 0;
		_buffer_data._data = nullptr;
		return data;
	}

	const unsigned char * buffer::get_data()const
	{
		return _buffer_data._data;
	}

	unsigned int buffer::get_length()const
	{
		return _buffer_data._lenght;
	}

	void buffer::alloc(const unsigned char * data_, unsigned int lenght_)
	{
		free();
		if (0 < lenght_)
		{
			_buffer_data._data = new unsigned char[lenght_];
			_buffer_data._lenght = lenght_;
			memcpy(_buffer_data._data, data_, _buffer_data._lenght);
		}
	}

	void buffer::free()
	{
		if(_buffer_data._data)
		{
			delete[] _buffer_data._data;
			_buffer_data._data = nullptr;
		}
		_buffer_data._lenght = 0;
	}

	bool buffer::is_empty()const
	{
		return (_buffer_data._lenght == 0);
	}

	unsigned char & buffer::operator[](const int index_)
	{
		return _buffer_data._data[index_];
	}

	buffer & buffer::operator=(const buffer & buffer_)
	{
		alloc(buffer_._buffer_data._data, buffer_._buffer_data._lenght);
		return *this;
	}
}