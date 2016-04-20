#pragma once

#include "video_settings.h"
#include <utility>
#include "../utils/buffer.h"

namespace webcam
{
	class image
	{
	public:
		image();
		image(const image & source_image_);	
		const image & operator=(const image & source_image_);
		~image();

		void copy_deep(const unsigned char * source_data_,
			unsigned int source_data_lenght_,
			const video_settings & settings_);

		void copy_shallow(unsigned char * source_data_,
			unsigned int source_data_lenght_,
			const video_settings & settings_);

		const video_settings & get_video_settings()const;
		const unsigned char * get_data()const;
		unsigned int get_data_lenght()const;
		bool is_empty()const;
		void empty();
		buffer_data detach();
		static image * create_empty();
	private:
		static void copy_data(
			const unsigned char * source_data_, 
			unsigned int source_data_lenght_,
			unsigned char *& target_data_, 
			unsigned int & target_data_lenght_
			);

		buffer _buffer;
		video_settings _video_settings;
	};

}