#pragma once

#include "VideoSettings.h"
#include <utility>
#include "utils/Buffer.h"

namespace webcam
{
	class Image
	{
	public:
		Image();
		Image(const Image & source_image_);
		const Image & operator=(const Image & source_image_);
		~Image();

		void copy_deep(const unsigned char * source_data_,
			unsigned int source_data_lenght_,
			const VideoSettings & settings_);

		void copy_shallow(unsigned char * source_data_,
			unsigned int source_data_lenght_,
			const VideoSettings & settings_);

		const VideoSettings & get_video_settings()const;
		const unsigned char * get_data()const;
		unsigned int get_data_lenght()const;
		bool is_empty()const;
		void empty();
		BufferData detach();
		static Image * create_empty();
	private:
		static void copy_data(
			const unsigned char * source_data_, 
			unsigned int source_data_lenght_,
			unsigned char *& target_data_, 
			unsigned int & target_data_lenght_
			);

		Buffer _buffer;
		VideoSettings _video_settings;
	};

}