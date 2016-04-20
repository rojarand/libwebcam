#pragma once

namespace webcam
{

	class buffer;
	class buffer_data
	{
		friend class buffer;
	public:
		buffer_data(unsigned char * data_, unsigned int lenght_);
		buffer_data();
		unsigned char * get_data();
		unsigned int get_lenght()const;
	private:
		unsigned char * _data;
		unsigned int _lenght;
	};

	class buffer
	{
	public:
		buffer();
		buffer(const unsigned char * data_, unsigned int len_);
		buffer(const buffer & buffer_);

		~buffer();
		void attach(const buffer_data & buffer_data_);
		buffer_data detach();
		const unsigned char * get_data()const;
		unsigned int get_length()const;
		
		void alloc(const unsigned char * data_, unsigned int len_);
		void free();
		bool is_empty()const;

		unsigned char & operator[](const int index_);
		buffer & operator=(const buffer & buffer_);
	protected:
		buffer_data _buffer_data;
	};

}
