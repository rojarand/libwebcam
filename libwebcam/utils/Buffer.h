#pragma once

namespace webcam
{

	class Buffer;
	class BufferData
	{
		friend class Buffer;
	public:
		BufferData(unsigned char * data_, unsigned int lenght_);
		BufferData();
		unsigned char * get_data();
		unsigned int get_lenght()const;
	private:
		unsigned char * _data;
		unsigned int _lenght;
	};

	class Buffer
	{
	public:
		Buffer();
		Buffer(const unsigned char * data_, unsigned int len_);
		Buffer(const Buffer & buffer_);

		~Buffer();
		void attach(const BufferData & buffer_data_);
		BufferData detach();
		const unsigned char * get_data()const;
		unsigned int get_length()const;
		
		void alloc(const unsigned char * data_, unsigned int len_);
		void free();
		bool is_empty()const;

		unsigned char & operator[](const int index_);
		Buffer & operator=(const Buffer & buffer_);
	protected:
		BufferData _buffer_data;
	};

}
